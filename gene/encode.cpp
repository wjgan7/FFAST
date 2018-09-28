#include <cstdio>
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

#define MAX_LENGTH 250000000
int N;
char seq[MAX_LENGTH];
int M;
bool code[MAX_LENGTH * 3];

void read_dna(std::string &file_name) {
  FILE* dna_file = std::fopen(file_name.c_str(), "r");
  std::fscanf(dna_file, "%s\n", seq);
  N = std::strlen(seq);
}

void write_with_N(char symbol, int pos) {
  if (symbol == 'a' || symbol == 'A') {
    code[pos] = code[pos+1] = code[pos+2] = 0;
  } else if (symbol == 'c' || symbol == 'C') {
    code[pos] = code[pos+1] = 0;
    code[pos+2] = 1;
  } else if (symbol == 'g' || symbol == 'G') {
    code[pos] = code[pos+2] = 0;
    code[pos+1] = 1;
  } else if (symbol == 't' || symbol == 'T') {
    code[pos] = 0;
    code[pos+1] = code[pos+2] = 1;
  } else {
    code[pos] = 1;
    code[pos+1] = code[pos+2] = 0;
  }
}

void write_without_N(char symbol, int pos) {
  if (symbol == 'a' || symbol == 'A') {
    code[pos] = code[pos+1] = 0;
  } else if (symbol == 'c' || symbol == 'C') {
    code[pos] = 0;
    code[pos+1] = 1;
  } else if (symbol == 'g' || symbol == 'G') {
    code[pos] = 1;
    code[pos+1] = 0;
  } else if (symbol == 't' || symbol == 'T') {
    code[pos] = 1;
    code[pos+1] = 1;
  }
}

void encode_with_N() {
  M = 3*N;
  for (int i = 0; i < N; i++) {
    write_with_N(seq[i], 3*i);
  }
}

void encode_without_N() {
  M = 2*N;
  for (int i = 0; i < N; i++) {
    write_without_N(seq[i], 2*i);
  }
}

void write_code(std::string &file_name) {
  FILE* code_file = std::fopen(file_name.c_str(), "w");
  std::fprintf(code_file, "%d\n", M);
  for (int i = 0; i < M; i++) {
    short val = code[i] ? 1 : -1;
    std::fprintf(code_file, "%d\n", val);
  }
}

int main() {
  for (int i = 0; i < 25; i++) {
    std::string name = seq_name(i);
    
    std::printf("Encoding sequence for %s\n", name.c_str());
    
    std::string dna_file = "grch38_dna/" + name + ".txt";
    std::string code_with_N_file = "grch38_code_with_N/" + name + ".txt";
    std::string code_without_N_file = "grch38_code_without_N/" + name + ".txt";
    
    std::printf("READING DNA from %s... ", dna_file.c_str());
    read_dna(dna_file);
    std::printf("Done.\n");
    
    std::printf("DNA IS OF LENGTH %d\n", N);
    
    std::printf("ENCODING DNA WITH N... ");
    encode_with_N();
    std::printf("Done.\n");

    std::printf("WRITING CODE WITH N TO %s... ", code_with_N_file.c_str());
    write_code(code_with_N_file);
    std::printf("Done.\n");

    std::printf("ENCODING DNA WITHOUT N... ");
    encode_without_N();
    std::printf("Done.\n");
    
    std::printf("WRITING CODE WITHOUT N TO %s...", code_without_N_file.c_str());
    write_code(code_without_N_file);
    std::printf("Done.\n");
  }
}
