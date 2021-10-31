#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>

#define NEUTRAL_ELEM 0

void propagate(std::vector<int>& bin_heap, unsigned int pos, unsigned int c){

    if (pos % 2 == 0){
        
        bin_heap[(c >> 1) + (pos >> 1)] = bin_heap[c + pos] xor bin_heap[c + pos + 1];
        
    } else{
        
        bin_heap[(c >> 1) + (pos >> 1)] = bin_heap[c + pos] xor bin_heap[c + pos - 1];
    }

    if (c > 1){

        propagate(bin_heap, pos >> 1, c >> 1);
    }
}

void insert(std::vector<int>& bin_heap, int new_elem, unsigned int pos, unsigned int c){

    bin_heap[c + pos] = new_elem;
    propagate(bin_heap, pos, c);
}

int find(std::vector<int>& bin_heap, unsigned int from, unsigned int to){

    int result = NEUTRAL_ELEM;
    if (from % 2 == 1) result = result xor bin_heap[from++];
    if (to % 2 == 0) result = result xor bin_heap[to--];
    
    if (to > from) result = result xor find(bin_heap, from >> 1, to >> 1);

    return result;
}

int main(){

    unsigned int n = 0, size = 1, new_elem = 0;

    std::cin >> n;

    while(size < n) size = size << 1; 
    std::vector<int> bin_heap(size << 1);

    for (int i = 0; i < n; i++){

        std::cin >> new_elem;
        insert(bin_heap, new_elem, i, size);
    }   

    find(bin_heap, size + 0, size + 0);
}