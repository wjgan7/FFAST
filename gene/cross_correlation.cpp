#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

std::string seq_name(int i) {
  static const char* last_names[3] = {"chrX", "chrY", "mito"};
  if (i < 22) {
    return "chr" + std::to_string(i+1);
  } else {
    return last_names[i-22];
  }
}

#define MAX_LENGTH 750000000

typedef long long ll;

int N, M, L;
bool seq[MAX_LENGTH];
/* ll cc[MAX_LENGTH]; */
ll max_cc[2000];

const int PATTERN_LENGTHS[4] = {50, 100, 500, 1000};
const int TRIALS_PER_SEQ = 10;

void cross_correlation(int start) {
  L = 0;
  ll running_max_cc = 1LL << 63;
  /* std::printf("%d\n", ); */
  int bucket_sz = std::max(1, N / 1000); 
  for (int i = 0; i < N-M+1; i++) {
    ll inner = 0;
    for (int j = 0; j < M; j++) {
      inner += seq[i+j] == seq[start+j] ? 1 : -1;
    }
    /* std::printf("%lld\n", inner); */

    /* cc[i] = inner; */
    running_max_cc = std::max(running_max_cc, inner);

    if (i % bucket_sz == bucket_sz-1) {
      max_cc[L] = running_max_cc;
      L += 1;
      running_max_cc = 1LL << 63;
    }
  }

  max_cc[L++] = running_max_cc;
}

void read_code(std::string &file_name) {
  FILE* code_file = std::fopen(file_name.c_str(), "r");
  std::fscanf(code_file, "%d\n", &N);
  for (int i = 0; i < N; i++) {
    int val;
    std::fscanf(code_file, "%d\n", &val);
    seq[i] = val == 1 ? 1 : 0;
  }
}

void write_cc(std::string &file_name) {
  FILE* cc_file = std::fopen(file_name.c_str(), "w");
  std::fprintf(cc_file, "%d\n", L);
  for (int i = 0; i < L; i++) {
    std::fprintf(cc_file, "%lld\n", max_cc[i]);
  }
}

int main() {
  for (int i = 24; i < 25; i++) {
    std::string name = seq_name(i);
    std::printf("Computing cross correlations for sequence %d\n", i+1);

    std::string code_with_N_file = "grch38_code_with_N/" + name + ".txt";
    
    std::printf("Reading code with N from %s... ", code_with_N_file.c_str());
    read_code(code_with_N_file); 
    std::printf("Done.\n");

    for (int j = 0; j < 4; j++) {
      M = PATTERN_LENGTHS[j];
      if (M >= N) {
        continue;
      }
      for (int k = 0; k < TRIALS_PER_SEQ; k++) {
        std::printf("Trial %d: computing cross correlation for code with N on "
                    "randomly selected pattern of length %d... ", k+1, M);
        cross_correlation(std::rand()%(N-M+1));
        std::printf("Done.\n");
        
        std::string cc_file = "grch38_cc_with_N/" + std::to_string(M) + "/" + 
            name + "_trial_" + std::to_string(k+1) + ".txt"; 
        
        std::printf("Trial %d: writing to file %s... ", k+1, cc_file.c_str());
        write_cc(cc_file);
        std::printf("Done.\n");
      }
    }
    
    std::string code_without_N_file = "grch38_code_without_N/" + name + ".txt";

    std::printf("Reading code without N from %s... ", code_with_N_file.c_str());
    read_code(code_without_N_file); 
    std::printf("Done.\n");
    
    for (int j = 0; j < 4; j++) {
      M = PATTERN_LENGTHS[j];
      for (int k = 0; k < TRIALS_PER_SEQ; k++) {
        std::printf("Trial %d: computing cross correlation for code without N "
                    "on randomly selected pattern of length %d... ", k+1, M); 
        cross_correlation(std::rand()%(N-M+1));
        std::printf("Done.\n");
        
        std::string cc_file = "grch38_cc_without_N/" + std::to_string(M) +
            "/" + name + "_trial_" + std::to_string(k+1) + ".txt"; 
        std::printf("Trial %d: writing to file %s... ", k+1, cc_file.c_str());
        write_cc(cc_file);
        std::printf("Done.\n");
      }
    }
  }
}

