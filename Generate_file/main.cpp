#include <random> 
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip> 
#include <unistd.h>

#define DATA_RANGE 200
#define NUM_OF_ITER 9000

#define MATRIX_SIZE 20
#define MATRIX_DETERM 2000
#define NUM_OF_PERMUT 300

void generate_array(bool compare, int argc, char** argv){

    bool* used_elems = nullptr;
    std::ofstream data_file;
    long range = DATA_RANGE, count_of_itter = NUM_OF_ITER, cur_num = 0, counter = 0;

    if (argc > 3){

        data_file.open(argv[1]);
        range = atoi(argv[2]);
        count_of_itter = atoi(argv[3]);
    } else{

        data_file.open("new_data.txt");
    }

    if (compare == true){

        used_elems = new bool[range];
    }

    std::mt19937 mersenne(static_cast<unsigned int>(time(0))); 

    for (int i = 0; i < count_of_itter; i++){

        cur_num = mersenne() % range;

        if (compare == true){

            if (!used_elems[cur_num]){

                data_file << cur_num << '\n';
                counter++;
                used_elems[cur_num] = true;
            }
        } else{

            data_file << cur_num << ' ';
            counter++;
        }
    }

    std::cout << "number of elements: " << counter << '\n';
    data_file << '\n' << counter;

    if (compare == true){

        delete[] used_elems;
    }
}


void generate_AVL_data(int argc, char** argv){

    bool* used_elems = nullptr;
    std::ofstream data_file, out_file;
    std::string out_file_name, data_file_name;
    long range = DATA_RANGE, cur_num = 0, counter = 0;    

    if (argc > 2){

        data_file_name.append(argv[1]);
        out_file_name.append(data_file_name, 0, data_file_name.find('.'));
        out_file_name.append("_out.txt");

        data_file.open(data_file_name);
        out_file.open(out_file_name, std::ios_base::trunc);
        range = atoi(argv[2]);
    } else{

        data_file_name.append("new_data.txt");
        data_file.open("new_data.txt", std::ios_base::trunc);
        out_file.open("new_data_out.txt", std::ios_base::trunc);
    }

    used_elems = new bool[range];
    std::mt19937 mersenne(static_cast<unsigned int>(time(0))); 

    while (counter < range){

        cur_num = mersenne() % range;

        if (!used_elems[cur_num]){

            data_file << "k " << cur_num << '\n';
            counter++;
            used_elems[cur_num] = true;
        }
    }
    counter = 0;

    while (counter < range){

        cur_num = mersenne() % range;

        if (used_elems[cur_num]){

            data_file << "m " << cur_num << '\n';
            data_file << "n " << cur_num << '\n';

            out_file << (cur_num - 1) << '\n';
            out_file << cur_num << '\n';
            used_elems[cur_num] = false;
            counter++;
        }      
    }

    data_file.close();
}


bool rearrange_lines(std::vector<int>& matrix, int fir_line, int sec_line){

    if (fir_line == sec_line){

        return false;
    }

    int n = sqrt(matrix.size());
    for (int i = 0; i < n; i++){

        std::swap(matrix[fir_line * n + i], matrix[sec_line * n + i]);
    }

    return true;
}

bool rearrange_columns(std::vector<int>& matrix, int fir_col, int sec_col){

    if (fir_col == sec_col){

        return false;
    }

    int n = sqrt(matrix.size());
    for (int i = 0; i < n; i++){

        std::swap(matrix[i * n + fir_col], matrix[i * n + sec_col]);
    }

    return true;
}

bool add_or_sub_lines(std::vector<int>& matrix, int fir_line, int sec_line, int op){

    if (fir_line == sec_line){

        return false;
    }
    
    int n = sqrt(matrix.size());
    if (op == 0){

        for (int i = 0; i < n; i++){

            matrix[fir_line * n + i] += matrix[sec_line * n + i];
        }
    } else{

        for (int i = 0; i < n; i++){

            matrix[fir_line * n + i] -= matrix[sec_line * n + i];
        }
    }
    return true;
}

bool add_or_sub_columns(std::vector<int>& matrix, int fir_col, int sec_col, int op){

    if (fir_col == sec_col){

        return false;
    }

    int n = sqrt(matrix.size());
    if (op == 0){

        for (int i = 0; i < n; i++){

            matrix[i * n + fir_col] += matrix[i * n + sec_col];
        }
    } else{

        for (int i = 0; i < n; i++){

            matrix[i * n + fir_col] -= matrix[i * n + sec_col];
        }
    }
    return true;
}

void generate_Matrix_data(int argc, char** argv){

    int matrix_size = 0, matrix_determ = 0;
    std::ofstream data_file;
    std::vector<int> matrix_diag_elems;
    
    if (argc > 3){

        data_file.open(argv[1]);
        matrix_size = atoi(argv[2]);
        matrix_determ = atoi(argv[3]);

    } else{

        data_file.open("new_data.txt");
        matrix_size = MATRIX_SIZE;
        matrix_determ = MATRIX_DETERM;
    }

    int cur_determ = matrix_determ;
    for (int i = 1; i <= matrix_determ; i++){

        if (cur_determ % i == 0){

            matrix_diag_elems.push_back(i);
            cur_determ /= i;
        }
    }

    while (matrix_diag_elems.size() < matrix_size){

        matrix_diag_elems.push_back(1);
    }
    
    
    std::vector<int> matrix(matrix_size * matrix_size);
    std::mt19937 mersenne(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < matrix_size; i++){

        matrix[i * matrix_size + i] = matrix_diag_elems[i];
    }

    for (int i = 0; i < NUM_OF_PERMUT; i++){


        switch (mersenne() % 4){
        
            case 0:
                rearrange_lines(matrix, mersenne() % matrix_size, mersenne() % matrix_size);
                break;
        
            case 1:
                rearrange_columns(matrix, mersenne() % matrix_size, mersenne() % matrix_size);
                break;

            case 2:
                add_or_sub_lines(matrix, mersenne() % matrix_size, mersenne() % matrix_size, mersenne() % 2);//тут проблемы
                break;

            case 3:
                add_or_sub_columns(matrix, mersenne() % matrix_size, mersenne() % matrix_size, mersenne() % 2);//тут проблемы
                break;
        }
    }

    data_file << matrix_determ << '\n';
    for (int i = 0; i < matrix_size; i++){

        for (int j = 0; j < matrix_size; j++){

            data_file << matrix[i * matrix_size + j] << ' ';
        }

        data_file << '\n';
    }
}


void print_matrix(std::vector<int> matrix){

    int n = sqrt(matrix.size()); 

    for (int i = 0; i < n; i++){

        for (int j = 0; j < n; j++){

            std::cout << matrix[i * n + j] << ' ';
        }
        std::cout << '\n';
    }
}

void test(){

    int n = 5;
    std::vector<int> matrix(n * n);

    for (int i = 0; i < n; i++){

        for (int j = 0; j < n; j++){

            std::cin >> matrix[i * n + j];
        }
    }

    print_matrix(matrix);

    std::cout << "-------\n";
    add_or_sub_columns(matrix, 0, 1, 1);

    print_matrix(matrix);
}

int main(int argc, char** argv){
    
    chdir("../outp");
    
    if (argc > 1){

        if (argv[1] == std::string("test")){

            test();
            exit(0);
        }

        if (argv[1] == std::string("AVL_tree")){

            argc--;
            argv = argv + 1;
            generate_AVL_data(argc, argv);
            exit(0);
        }

        if (argv[1] == std::string("Matrix")){

            argc--;
            argv = argv + 1;
            generate_Matrix_data(argc, argv);
            exit(0);
        }

        if (argv[1] == std::string("distinct")){

            argc--;
            argv = argv + 1;
            generate_array(true, argc, argv);
            exit(0);
        }
    }

    generate_array(false, argc, argv);
}