#include <cassert>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <unistd.h>

#define MAXLEN 10002
#define BUFFER_SIZE 16

struct Buffer{

    char* data[BUFFER_SIZE];
    int cur_size;
};

struct Line{

    FILE* file;
    int cur_arr_position;
    char cur_elem[MAXLEN];

    void init(FILE* new_file){

        file = new_file;
        cur_arr_position = -1;
    }

    void set_zero(){
        
        cur_arr_position = -1;
    }

    void go_to_beggining(){

        fseek(file, SEEK_SET, 0);
        cur_arr_position = -1;
    }

    void add_elem(const char* new_str){

        fputs(new_str, file);
        cur_arr_position++;
    }

    const char* get_elem(int position){

        if (cur_arr_position == -1){

            if (fgets(cur_elem, MAXLEN, file) != 0){

                cur_arr_position++;
            } else{

                printf("End of line\n");
                return nullptr;
            }
        }

        if (position == cur_arr_position){

            return cur_elem;
        }

        if (position - 1 == cur_arr_position){
            
            if (fgets(cur_elem, MAXLEN, file) != 0){

                cur_arr_position++;
                return cur_elem;
            } else{

                printf("End of line\n");
                return nullptr;
            }
        }

        printf("incorrect line move\n");
        return nullptr;
    } 

    void print(FILE* output){

        int old_position = ftell(file);
        char tmp_str[MAXLEN];

        fseek(file, SEEK_SET, 0);
        while (fgets(tmp_str, MAXLEN, file) != 0){

            fputs(tmp_str, output);
        }
        
        fseek(file, SEEK_SET, old_position);
    }
};

int compare_string(const void* fir_str, const void* sec_str){

    return strcmp(*(const char**)fir_str, *(const char**)sec_str);
}

void put_buffer_to(Buffer& buf, Line& file){

   for (int i = 0; i < buf.cur_size; i++){

       file.add_elem(buf.data[i]);
    }
}

int buffer_sort(FILE* input_file, Line& first_line, Line& second_line){

    int itter = 0, cur_line = 1, number_of_str = 0;
    bool end_of_file = false;
    Buffer buffer;
    buffer.cur_size = 0;

    for (int i = 0; i < BUFFER_SIZE; i++){

        buffer.data[i] = new char[MAXLEN];
    }

    while (!end_of_file){

        if (fgets(buffer.data[itter], MAXLEN, input_file) != 0){

            number_of_str++;
            buffer.cur_size++;

            if (itter < BUFFER_SIZE - 1){
                
                itter++;
            } else{

                itter = 0;

                if (cur_line == 1){

                    qsort(buffer.data, buffer.cur_size, sizeof(char*), compare_string);
                    put_buffer_to(buffer, first_line);
                    cur_line = 2;
                } else{

                    qsort(buffer.data, buffer.cur_size, sizeof(char*), compare_string);
                    put_buffer_to(buffer, second_line);
                    cur_line = 1;
                }
                
                buffer.cur_size = 0;
            }
        } else{

            end_of_file = true;

            if (cur_line == 1){

                qsort(buffer.data, buffer.cur_size, sizeof(char*), compare_string);
                put_buffer_to(buffer, first_line);
                cur_line = 2;
            } else{

                qsort(buffer.data, buffer.cur_size, sizeof(char*), compare_string);
                put_buffer_to(buffer, second_line);
                cur_line = 1;
            }
        }
    }

    return number_of_str;
}

void merge(Line& first_line, Line& second_line, FILE* output, int first_line_len, int second_line_len){

    int fir_itter = 0, sec_itter = 0;
    first_line.set_zero();
    second_line.set_zero();

    while ((fir_itter < first_line_len) && (sec_itter < second_line_len)){
        
        if (strcmp(first_line.get_elem(fir_itter), second_line.get_elem(sec_itter)) < 0){
            
            fputs(first_line.get_elem(fir_itter), output); 
            fir_itter++;
        } else{

            fputs(second_line.get_elem(sec_itter), output);
            sec_itter++;
        }
    }

    if (fir_itter != first_line_len){

        while (fir_itter < first_line_len){
            
            fputs(first_line.get_elem(fir_itter), output);
            fir_itter++; 
        }
    } else{

        while (sec_itter < second_line_len){

            fputs(second_line.get_elem(sec_itter), output);
            sec_itter++; 
        }
    }   
}

int put_str_to(FILE* input, Line& line, int cur_series_len){

    char cur_str[MAXLEN];
    int i = 0;

    line.set_zero();

    for (i = 0; i < cur_series_len; i++){

        fgets(cur_str, MAXLEN, input);
        line.add_elem(cur_str);
    }

    return i;
}

void split_into_lines(Line& first_line, Line& second_line, FILE* output, int cur_series_len, int number_of_str){

    int cur_line = 1;
    int number_of_chunks = 0, small_chunk = 0;

    first_line.go_to_beggining();
    second_line.go_to_beggining();

    fseek(output, SEEK_SET, 0);

    number_of_chunks = number_of_str / cur_series_len;
    small_chunk = number_of_str % cur_series_len;

    while (number_of_chunks > 0){
            
        if (cur_line == 1){

            put_str_to(output, first_line, cur_series_len);
            cur_line = 2;
        } else{

            put_str_to(output, second_line, cur_series_len);
            cur_line = 1;
        }

        number_of_chunks--;
    }

    if (small_chunk != 0){

        if (cur_line == 1){

            put_str_to(output, first_line, small_chunk);
        } else{

            put_str_to(output, second_line, small_chunk);
        }
    }
}

void merge_sort(Line& first_line, Line& second_line, FILE* output, int number_of_str){

    int cur_series_len = BUFFER_SIZE;
    int first_line_len, second_line_len, number_of_chunks = number_of_str / cur_series_len, small_chunk = 0, elements_in_first = 0, elements_in_second = 0;
    
    while (cur_series_len <= 2 * number_of_str){

        number_of_chunks = number_of_str / cur_series_len;
        small_chunk = number_of_str % cur_series_len;

        first_line.go_to_beggining();
        second_line.go_to_beggining();
        fseek(output, SEEK_SET, 0);

        while (number_of_chunks >= 0){ 
            
            if (number_of_chunks >= 2){

                merge(first_line, second_line, output, cur_series_len, cur_series_len);
                number_of_chunks -= 2;
            } else{
                
                if (number_of_chunks == 1){

                    merge(first_line, second_line, output, cur_series_len, small_chunk);

                    number_of_chunks -= 2;
                } else{

                    merge(first_line, second_line, output, small_chunk, 0);
                    number_of_chunks--;
                }                
            }
        }
        
        cur_series_len *= 2;

        if (number_of_chunks < number_of_str){

            split_into_lines(first_line, second_line, output, cur_series_len, number_of_str);
        }   
    }
}

int main(){
    
    FILE* input_file = fopen("input.txt", "r");
    
    FILE* first_line_file = fopen("first_line.txt", "w+t");
    FILE* second_line_file = fopen("second_line.txt", "w+t");
    FILE* output_file = fopen("output.txt", "w+t");

    assert(first_line_file != nullptr);
    assert(second_line_file != nullptr);
    assert(output_file != nullptr);

    Line first_line, second_line;

    first_line.init(first_line_file);
    second_line.init(second_line_file);
    
    int number_of_lines = buffer_sort(input_file, first_line, second_line);        

    if (BUFFER_SIZE <= number_of_lines){

        merge_sort(first_line, second_line, output_file, number_of_lines);
    } else{

        first_line.print(output_file);
    }
    
    fclose(output_file);
    fclose(first_line_file);
    fclose(second_line_file); 

    unlink("first_line.txt");
    unlink("second_line.txt");
}

