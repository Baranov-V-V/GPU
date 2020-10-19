#pragma once

#include <stdio.h>
#include <math.h>
#include <stdint.h>

static const int MAX_NAME_LEN = 100;

static char file_in[MAX_NAME_LEN];

void Hlt_(struct StackArray* stack);

void In_(struct StackArray* stack);

void Out_(struct StackArray* stack);

void Pop_(struct StackArray* stack);

void Add_(struct StackArray* stack);

void Div_(struct StackArray* stack);

void Mul_(struct StackArray* stack);

void Sub_(struct StackArray* stack);

void Sqr_(struct StackArray* stack);

void Sqrt_(struct StackArray* stack);

void Cos_(struct StackArray* stack);

void Sin_(struct StackArray* stack);

void Push_(struct StackArray* stack, FILE* fp_in);

void Calculate(struct StackArray* stack, FILE* fp_in);
