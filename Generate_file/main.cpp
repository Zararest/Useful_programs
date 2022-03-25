#include <random> 
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip> 
#include <unistd.h>
#include <cassert>
#include <set>

#define DATA_RANGE 200
#define NUM_OF_ITER 9000

#define MATRIX_SIZE 20
#define MATRIX_DETERM 2000
#define NUM_OF_PERMUT 90000
#define EXTRA_SIZE 3

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

            data_file << cur_num << '\n';
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

    delete[] used_elems;
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


void generate_Splay_find(int argc, char** argv){

    bool* used_elems = nullptr;
    std::ofstream data_file, req_file, ans_file;
    std::string data_file_name, req_file_name, ans_file_name;

    if (argc > 3){

        data_file_name.append("input_");
        data_file_name.append(argv[1]);

        req_file_name.append("requests_");
        req_file_name.append(argv[1]);

        ans_file_name.append("ans_");
        ans_file_name.append(argv[1]);

        argc--;
        argv = argv + 1;
    } else{

        data_file_name.append("input");
        req_file_name.append("requests");
        ans_file_name.append("ans");
    }

    data_file.open(data_file_name);
    req_file.open(req_file_name);
    ans_file.open(ans_file_name);

    assert(data_file.is_open() && req_file.is_open() && ans_file.is_open());

    if (argc <= 2){ std::cout << "Not enoght args" << std::endl; return;}

    int num_of_elems = atoi(argv[1]);
    int num_of_req = atoi(argv[2]);

    int* arr_of_nums = new int[num_of_elems * EXTRA_SIZE];
    std::set<int> new_set;
    std::mt19937 mersenne(static_cast<unsigned int>(time(0))); 

    for (int i = 0; i < num_of_elems * EXTRA_SIZE; i++){

        arr_of_nums[i] = i;
    }

    for (int i = 0; i < NUM_OF_PERMUT; i++){

        std::swap(arr_of_nums[mersenne() % (num_of_elems * EXTRA_SIZE)], arr_of_nums[mersenne() % (num_of_elems * EXTRA_SIZE)]);
    }

    for (int i = 0; i < num_of_elems; i++){

        data_file << arr_of_nums[i] << std::endl;
        new_set.insert(arr_of_nums[i]);
    }

    int cur_req = 0;

    for (int i = 0; i < num_of_req; i++){

        cur_req = mersenne() % (num_of_elems * 3);
        req_file << cur_req << std::endl;
        ans_file << (new_set.find(cur_req) != new_set.end()) << std::endl;
    }

    data_file.close();
    req_file.close();
    ans_file.close();

    delete[] arr_of_nums;
}   


int get_number_of_elems_in_set(std::set<int>& cur_set, int from, int to){

    int ret_val = 0;

    for (int i = from; i <= to; i++){

        if (cur_set.find(i) != cur_set.end()){ ret_val++; }
    }

    return ret_val;
}

void generate_Splay_range(int argc, char** argv){

    bool* used_elems = nullptr;
    std::ofstream data_file, req_file, ans_file;
    std::string data_file_name, req_file_name, ans_file_name;

    if (argc > 3){

        data_file_name.append("input_");
        data_file_name.append(argv[1]);

        req_file_name.append("requests_");
        req_file_name.append(argv[1]);

        ans_file_name.append("ans_");
        ans_file_name.append(argv[1]);

        argc--;
        argv = argv + 1;
    } else{

        data_file_name.append("input");
        req_file_name.append("requests");
        ans_file_name.append("ans");
    }

    data_file.open(data_file_name);
    req_file.open(req_file_name);
    ans_file.open(ans_file_name);

    assert(data_file.is_open() && req_file.is_open() && ans_file.is_open());

    if (argc <= 2){ std::cout << "Not enoght args" << std::endl; return;}

    int num_of_elems = atoi(argv[1]);
    int num_of_req = atoi(argv[2]);

    int* arr_of_nums = new int[num_of_elems * EXTRA_SIZE];
    std::set<int> new_set;
    std::mt19937 mersenne(static_cast<unsigned int>(time(0))); 

    for (int i = 0; i < num_of_elems * EXTRA_SIZE; i++){

        arr_of_nums[i] = i;
    }

    for (int i = 0; i < NUM_OF_PERMUT; i++){

        std::swap(arr_of_nums[mersenne() % (num_of_elems * EXTRA_SIZE)], arr_of_nums[mersenne() % (num_of_elems * EXTRA_SIZE)]);
    }

    for (int i = 0; i < num_of_elems; i++){

        data_file << arr_of_nums[i] << std::endl;
        new_set.insert(arr_of_nums[i]);
    }

    int cur_left_lim = 0, cur_right_lim = 0;

    for (int i = 0; i < num_of_req; i++){

        cur_left_lim = mersenne() % (num_of_elems * 2);
        cur_right_lim = cur_left_lim + (mersenne() % num_of_elems);
        req_file << cur_left_lim << ' ' << cur_right_lim << std::endl;
        ans_file << get_number_of_elems_in_set(new_set , cur_left_lim, cur_right_lim) << std::endl;
    }

    data_file.close();
    req_file.close();
    ans_file.close();

    delete[] arr_of_nums;
}

int get_elem(std::set<int>& cur_set, int num_of_elem){

    std::set<int>::iterator iter = cur_set.begin();

    for (int i = 0; i < num_of_elem; i++){

        ++iter;
    }

    return *iter;
}

void generate_Splay_Gaus(int argc, char** argv){

    std::ofstream data_file, req_file, ans_file;
    std::string data_file_name, req_file_name, ans_file_name;

    if (argc > 3){

        data_file_name.append("input_");
        data_file_name.append(argv[1]);

        req_file_name.append("requests_");
        req_file_name.append(argv[1]);

        ans_file_name.append("ans_");
        ans_file_name.append(argv[1]);

        argc--;
        argv = argv + 1;
    } else{

        data_file_name.append("input");
        req_file_name.append("requests");
        ans_file_name.append("ans");
    }

    data_file.open(data_file_name);
    req_file.open(req_file_name);
    ans_file.open(ans_file_name);

    assert(data_file.is_open() && req_file.is_open() && ans_file.is_open());

    if (argc <= 3){ std::cout << "Not enoght args" << std::endl; return;}

    int num_of_elems = atoi(argv[1]);
    int num_of_req = atoi(argv[2]);
    double sigma = strtod(argv[3], nullptr);

    int* arr_of_nums = new int[num_of_elems * EXTRA_SIZE];
    std::set<int> new_set;
    std::mt19937 mersenne(static_cast<unsigned int>(time(0))); 

    for (int i = 0; i < num_of_elems * EXTRA_SIZE; i++){

        arr_of_nums[i] = i;
    }

    for (int i = 0; i < NUM_OF_PERMUT; i++){

        std::swap(arr_of_nums[mersenne() % (num_of_elems * EXTRA_SIZE)], arr_of_nums[mersenne() % (num_of_elems * EXTRA_SIZE)]);
    } 

    for (int i = 0; i < num_of_elems; i++){

        data_file << arr_of_nums[i] << std::endl;
        new_set.insert(arr_of_nums[i]);
    }

    int cur_median = 0, cur_size = 0;
    int cur_left_lim = 0, cur_right_lim = 0;

    std::normal_distribution<> normal_dist{num_of_elems / 2, sigma};

    for (int i = 0; i < num_of_req; i++){

        cur_median = normal_dist(mersenne);
        cur_median = cur_median % (num_of_elems / 2);
        cur_size = mersenne() % num_of_elems;

        if (cur_median - cur_size < 0){

            cur_left_lim = 0;
        } else{
            
            cur_left_lim = get_elem(new_set, cur_median - cur_size);
        }

        if (cur_median + cur_size >= num_of_elems){
            
            cur_right_lim = get_elem(new_set, new_set.size() - 1);
        } else{

            cur_right_lim = get_elem(new_set, cur_median + cur_size);
        }

        req_file << cur_left_lim << ' ' << cur_right_lim << std::endl;
        ans_file << get_number_of_elems_in_set(new_set , cur_left_lim, cur_right_lim) << std::endl;
    }

    delete[] arr_of_nums;
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

        if (argv[1] == std::string("Splay_find")){

            argc--;
            argv = argv + 1;
            generate_Splay_find(argc, argv);
            exit(0);
        }

        if (argv[1] == std::string("Splay_number_of_elems")){

            argc--;
            argv = argv + 1;
            generate_Splay_range(argc, argv);
            exit(0);
        }

        if (argv[1] == std::string("Splay_Gaus")){

            argc--;
            argv = argv + 1;
            generate_Splay_Gaus(argc, argv);
            exit(0);
        }
    }

    generate_array(false, argc, argv);
}