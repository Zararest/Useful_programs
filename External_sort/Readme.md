# Программа для сортировки текстовых файлов любого размера
В данной программе используется метод внешней сортировки, в которой используются дополнительные текстовые файлы для хранения промежуточных стади сортировки. Алгоритм требует количество памяти равное максимальному объему строки. В начале проводиться сортировка с помощью qsort()(объем сортируемой памяти регулируется константой BUFFER_SIZE), затем применяется сортировка слиянием.