#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

int matrix_size;
vector<vector<int>> matrix_a;
vector<vector<int>> matrix_b;
vector<vector<int>> result_matrix;

void calculate_row(int row_index) {
    int matrix_size = matrix_a.size();
    for (int col_index = 0; col_index < matrix_size; ++col_index) {
        for (int k = 0; k < matrix_size; ++k) {
            result_matrix[row_index][col_index] += matrix_a[row_index][k] * matrix_b[k][col_index];
        }
    }
}

void add_matrices_part(int start_row, int end_row) {
    int matrix_size = matrix_a.size();
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            result_matrix[i][j] = matrix_a[i][j] + matrix_b[i][j];
        }
    }
}

void subtract_matrices_part(int start_row, int end_row) {
    int matrix_size = matrix_a.size();
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            result_matrix[i][j] = matrix_a[i][j] - matrix_b[i][j];
        }
    }
}

void save_matrix_to_file(const vector<vector<int>>& matrix, const string& MatrixFile) {
    ofstream file(MatrixFile, ios::out | ios::trunc);
    if (file.is_open()) {
        for (const auto& row : matrix) {
            for (int value : row) {
                file << value << " ";
            }
            file << "\n";
        }
        file.close();
        cout << "Matrix saved to " << MatrixFile << endl;
    } else {
        cerr << "Error opening file for writing: " << MatrixFile << endl;
    }
}

void save_value_to_file(int value, const string& MatrixFile) {
    ofstream file(MatrixFile, ios::out | ios::trunc);
    if (file.is_open()) {
        file << value << endl;
        file.close();
        cout << "Value saved to " << MatrixFile << endl;
    } else {
        cerr << "Error opening file for writing: " << MatrixFile << endl;
    }
}

int determinant_A(const vector<vector<int>>& matrix);
int determinant_B(const vector<vector<int>>& matrix);

void display_matrix(const vector<vector<int>>& matrix) {
    int matrix_size = matrix.size();
    cout << "Resulting matrix (" << matrix_size << "x" << matrix_size << "):" << endl;
    for (const auto& row : matrix) {
        for (int value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

int determinant_A(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    vector<vector<int>> sub_matrix(n - 1, vector<int>(n - 1));
    int det = 0;
    int sign = 1;

    if (n == 1) {
        return matrix[0][0];
    }

    for (int col = 0; col < n; ++col) {
        int sub_i = 0, sub_j = 0;
        for (int i = 1; i < n; ++i) {
            sub_j = 0;
            for (int j = 0; j < n; ++j) {
                if (j != col) {
                    sub_matrix[sub_i][sub_j++] = matrix[i][j];
                }
            }
            ++sub_i;
        }
        det += sign * matrix[0][col] * determinant_A(sub_matrix);
        sign = -sign;
    }

    return det;
}

int determinant_B(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    vector<vector<int>> sub_matrix(n - 1, vector<int>(n - 1));
    int det = 0;
    int sign = 1;

    if (n == 1) {
        return matrix[0][0];
    }

    for (int col = 0; col < n; ++col) {
        int sub_i = 0, sub_j = 0;
        for (int i = 1; i < n; ++i) {
            sub_j = 0;
            for (int j = 0; j < n; ++j) {
                if (j != col) {
                    sub_matrix[sub_i][sub_j++] = matrix[i][j];
                }
            }
            ++sub_i;
        }
        det += sign * matrix[0][col] * determinant_B(sub_matrix);
        sign = -sign;
    }

    return det;
}


int main() {
    srand(time(nullptr));

    cout << "Enter matrix size: ";
    cin >> matrix_size;

    matrix_a.resize(matrix_size, vector<int>(matrix_size));
    matrix_b.resize(matrix_size, vector<int>(matrix_size));
    result_matrix.resize(matrix_size, vector<int>(matrix_size, 0));

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            matrix_a[i][j] = rand() % 100;
            matrix_b[i][j] = rand() % 100;
        }
    }

    string matrixAFilePath = "C:\\CLionProjects\\MatrixThreads\\MatrixA.txt";
    string matrixBFilePath = "C:\\CLionProjects\\MatrixThreads\\MatrixB.txt";
    string resultMatrixFilePath = "C:\\CLionProjects\\MatrixThreads\\ResultMatrix.txt";

    save_matrix_to_file(matrix_a, matrixAFilePath);
    save_matrix_to_file(matrix_b, matrixBFilePath);

    int choice;
    bool exitProgram = false;

    while (!exitProgram) {
        cout << "Choose an operation:\n";
        cout << "1. Matrix Addition\n";
        cout << "2. Matrix Subtraction\n";
        cout << "3. Matrix Multiplication\n";
        cout << "4. Calculate Determinant of Matrix A\n";
        cout << "5. Calculate Determinant of Matrix B\n";
        cout << "6. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1: {
                vector<thread> threads;
                for (int i = 0; i < matrix_size; ++i) {
                    int start_row = i;
                    int end_row = i + 1;
                    threads.emplace_back(add_matrices_part, start_row, end_row);
                }
                for (auto& thread : threads) {
                    thread.join();
                }
                save_matrix_to_file(result_matrix, resultMatrixFilePath);
                display_matrix(result_matrix);
                break;
            }
            case 2: {
                vector<thread> threads;
                for (int i = 0; i < matrix_size; ++i) {
                    int start_row = i;
                    int end_row = i + 1;
                    threads.emplace_back(subtract_matrices_part, start_row, end_row);
                }
                for (auto& thread : threads) {
                    thread.join();
                }
                save_matrix_to_file(result_matrix, resultMatrixFilePath);
                display_matrix(result_matrix);
                break;
            }
            case 3: {
                vector<thread> threads;
                for (int i = 0; i < matrix_size; ++i) {
                    threads.emplace_back(calculate_row, i);
                }
                for (auto& thread : threads) {
                    thread.join();
                }
                save_matrix_to_file(result_matrix, resultMatrixFilePath);
                display_matrix(result_matrix);
                break;
            }
            case 4: {
                int detA = determinant_A(matrix_a);
                cout << "Determinant of Matrix A: " << detA << endl;
                save_value_to_file(detA, resultMatrixFilePath);
                break;
            }
            case 5: {
                int detB = determinant_B(matrix_b);
                cout << "Determinant of Matrix B: " << detB << endl;
                save_value_to_file(detB, resultMatrixFilePath);
                break;
            }
            case 6:
                exitProgram = true;
                break;
            default:
                cerr << "Invalid choice." << endl;
                break;
        }
    }

    return 0;
}

