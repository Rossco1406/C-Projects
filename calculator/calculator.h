struct calculator {
    char operators[STACK_SIZE];
    double values[STACK_SIZE];

    int operator_sp;
    int value_sp;
};