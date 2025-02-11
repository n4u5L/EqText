#include <stdio.h>

int a[6][7] = {{0, 0, 1, 0, 1, 1, 0}, {1, 0, 0, 1, 0, 0, 1},
               {0, 1, 1, 0, 0, 1, 0}, {1, 0, 0, 1, 0, 0, 0},
               {0, 1, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 1, 0, 1}};

void print_mat(int* mat, int r, int c) {
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      printf("%d ", mat[i * c + j]);
    }
    printf("\n");
  }
}

void exact_cover_problem(int* mat, int r, int c) {
  print_mat(mat, r, c);

  int find = 0;
  for (int state = 1; state < (1 << r); ++state) {
    int j_flag = 1;
    for (int j = 0; j < c; ++j) {
      int r_flag = 0;
      for (int i = 0; i < r; ++i) {
        if ((1 << i) & state) {
          if (mat[i * c + j])
            ++r_flag;
        }
      }
      if (r_flag != 1)
        j_flag = 0;
    }

    if (j_flag) {
      find = 1;
      printf("line: ");
      for (int i = 0; i < r; ++i) {
        if ((1 << i) & state) {
          printf("%d ", i + 1);
        }
      }
      printf("\n");
    }
  }

  if (!find)
    printf("none!\n");
}

int main() {
  exact_cover_problem(a[0], 6, 7);
}