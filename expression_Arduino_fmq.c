#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression_fmq.h"

#define ERROR -100000

// void split(char *tmp, char *string, unsigned char start, unsigned char end)
// {
//     for (unsigned char i = start; i < end; ++i)
//     {
//         tmp[i - start] = string[i];
//     }
// }

int Is_Format(char *string, int len) //判断结构是否匹配
{
    int tmp_circle = 0, tmp_square = 0;
    for (int i = 0; i < len; ++i)
    {
        switch (string[i])
        {
        case '(':
            tmp_circle++;
            break;
        case ')':
            tmp_circle--;
            if (tmp_circle < 0)
                return 0;
            break;
        case '[':
            tmp_square++;
            break;
        case ']':
            tmp_square--;
            if (tmp_square < 0)
                return 0;
            break;
            break;
        default:
            break;
        }
    }
    return !(tmp_circle + tmp_square); //递归底层返回op //>123<
}

int Get_Double_Left(char *string, int len, int local) //获取二元运算符左边
{
    if (local <= 0 || local > len) // _+>123456+_<
        return -1;
    int tmp_bit = string[local - 1];
    if ((tmp_bit > 47 && tmp_bit < 58) || tmp_bit == 46) // 0123456789.
    {
        for (int i = local - 2; i > -1; --i)
        {
            if (!((string[i] > 47 && string[i] < 58) || string[i] == 46)) //! 0123456789.
                return i + 1;
        }
    }
    else if ((tmp_bit > 64 && tmp_bit < 71) || tmp_bit == 77 || tmp_bit == 88 || tmp_bit == 89 || tmp_bit == 101 || tmp_bit == 110)
        return local - 1; // ABCDEFMXY
    else if (tmp_bit == ')' || tmp_bit == ']')
    {
        int tmp = -1;
        for (int i = local - 2; i > -1; --i)
        {
            switch (string[i])
            {
            case '(':
                tmp++;
                break;
            case ')':
                tmp--;
                break;
            case '[':
                tmp++;
                break;
            case ']':
                tmp--;
                break;
            default:
                break;
            }
            if (!tmp)
            {
                return i;
            }
        }
    }
    else
        return ERROR; //左边无操作符
    return 0;         //左边只有一位
}

int Get_Double_Right(char *string, int len, int local) //获取二元运算符右边
{
    if (local < -1 || local >= len - 1) // >_+123456<+_
        return -1;
    int tmp_bit = string[local + 1];
    if ((tmp_bit > 47 && tmp_bit < 58) || tmp_bit == 46) // 0123456789.
    {
        for (int i = local + 2; i < len; ++i)
        {
            if (!((string[i] > 47 && string[i] < 58) || string[i] == 46)) //! 0123456789.
            {
                return i - 1;
            }
        }
    }
    else if ((tmp_bit > 64 && tmp_bit < 71) || tmp_bit == 77 || tmp_bit == 88 || tmp_bit == 89 || tmp_bit == 101 || tmp_bit == 110)
        return local + 1;                      // ABCDEFMXY
    else if (tmp_bit == '(' || tmp_bit == '[') // ()[]
    {
        int tmp = 1;
        for (int i = local + 2; i < len; ++i)
        {
            // printf("%c", string[i]);
            switch (string[i])
            {
            case '(':
                tmp++;
                break;
            case ')':
                tmp--;
                break;
            case '[':
                tmp++;
                break;
            case ']':
                tmp--;
                break;
            default:
                break;
            }
            if (!tmp)
            {
                return i;
            }
        }
    }
    else
        return ERROR; //右边无单操作符
    return len - 1;   //右边只有一位
}

op *analyze(char *string, int len)
{
    for (int i = 0, tmp_circle = 0, tmp_square = 0; i < len; ++i) // +-
    {
        switch (string[i])
        {
        case '(':
            tmp_circle++;
            break;
        case ')':
            tmp_circle--;
            break;
        case '[':
            tmp_square++;
            break;
        case ']':
            tmp_square--;
            break;
        default:
            if (!(tmp_circle + tmp_square))
            {
                switch (string[i])
                {
                case '+': // A+B
                {
                    op *tmp_op = new_op(string + i, 1);
                    // tmp_op->child[0] = analyze(string + Get_Double_Left(string, len, i), i - Get_Double_Left(string, len, i));
                    // tmp_op->child[1] = analyze(string + i + 1, Get_Double_Right(string, len, i) - i);
                    tmp_op->child[0] = analyze(string, i);
                    tmp_op->child[1] = analyze(string + i + 1, len - i - 1);
                    return tmp_op;
                }
                case '-': // A-B
                {
                    op *tmp_op = new_op(string + i, 1);
                    // tmp_op->child[0] = analyze(string + Get_Double_Left(string, len, i), i - Get_Double_Left(string, len, i));
                    // tmp_op->child[1] = analyze(string + i + 1, Get_Double_Right(string, len, i) - i);
                    tmp_op->child[0] = analyze(string, i);
                    tmp_op->child[1] = analyze(string + i + 1, len - i - 1);
                    return tmp_op;
                }
                default:
                    break;
                }
            }
            break;
        }
    }
    for (int i = 0, tmp_circle = 0, tmp_square = 0; i < len; ++i) // */
    {
        switch (string[i])
        {
        case '(':
            tmp_circle++;
            break;
        case ')':
            tmp_circle--;
            break;
        case '[':
            tmp_square++;
            break;
        case ']':
            tmp_square--;
            break;
        default:
            if (!(tmp_circle + tmp_square))
            {
                switch (string[i])
                {
                case '*': // A+B
                {
                    op *tmp_op = new_op(string + i, 1);
                    // tmp_op->child[0] = analyze(string + Get_Double_Left(string, len, i), i - Get_Double_Left(string, len, i));
                    // tmp_op->child[1] = analyze(string + i + 1, Get_Double_Right(string, len, i) - i);
                    tmp_op->child[0] = analyze(string, i);
                    tmp_op->child[1] = analyze(string + i + 1, len - i - 1);
                    return tmp_op;
                }
                default:
                    break;
                }
            }
            break;
        }
    }
    for (int i = 0, tmp_circle = 0, tmp_square = 0; i < len; ++i) // */
    {
        switch (string[i])
        {
        case '(':
            tmp_circle++;
            break;
        case ')':
            tmp_circle--;
            break;
        case '[':
            tmp_square++;
            break;
        case ']':
            tmp_square--;
            break;
        default:
            if (!(tmp_circle + tmp_square))
            {
                switch (string[i])
                {
                case '/': // A-B
                {
                    op *tmp_op = new_op(string + i, 1);
                    // tmp_op->child[0] = analyze(string + Get_Double_Left(string, len, i), i - Get_Double_Left(string, len, i));
                    // tmp_op->child[1] = analyze(string + i + 1, Get_Double_Right(string, len, i) - i);
                    tmp_op->child[0] = analyze(string, i);
                    tmp_op->child[1] = analyze(string + i + 1, len - i - 1);
                    return tmp_op;
                }
                default:
                    break;
                }
            }
            break;
        }
    }

    if (string[0] == '(')
        return analyze(string + 1, len - 2);
    else if (string[0] == '[')
    {

        if (string[1] == '_')
        {
            op *tmp_op = new_op(string + 1, 2);
            tmp_op->child[0] = analyze(string + 4, len - 5);
            return tmp_op;
        }
        else if (string[1] == '|' || string[1] == 'n' || string[1] == '!' || string[1] == 's' || string[1] == 's' || string[1] == 'c' || string[1] == 't' || string[1] == '%')
        {
            op *tmp_op = new_op(string + 1, 1);
            tmp_op->child[0] = analyze(string + 3, len - 4);
            return tmp_op;
        }
        else if (string[1] == '\\' || string[1] == '^' || string[1] == '`' || string[1] == '~')
        {
            op *tmp_op = new_op(string + 1, 1);
            for (int i = 3; i < len; ++i)
            {
                if (string[i] == ';')
                {
                    tmp_op->child[0] = analyze(string + 3, i - 2);
                    tmp_op->child[1] = analyze(string + i + 1, len - i - 2);
                    return tmp_op;
                }
            }
        }
    }
    else
        return new_op(string, len);
    // for (int i = 0, tmp = 0; i < len + 1; ++i)
    // {
    //     if (string[i] == '(') //累加左括号数量
    //     {
    //         tmp++;
    //     }
    //     else if (string[i] == ')') //累减右括号数量
    //     {
    //         tmp--;    //积累括号
    //         if (!tmp) //左右括号匹配
    //         {
    //             op *tmp_ch = analyze(string + 1, i - 1); //左表达式递归解析 // (>(a)+(b)<)+...
    //             char tmp_symbol = string[i + 1];
    //             op *tmp_op = new_op(string + i + 1, 1); //本层级运算符 // ((a)+(b))>+<...
    //             if (tmp_symbol == 's' || tmp_symbol == 'c' || tmp_symbol == 'g')
    //             {
    //                 tmp_op->child[0] = tmp_ch;
    //             }
    //             else
    //             {
    //                 tmp_op->child[0] = tmp_ch;
    //                 tmp_op->child[1] = analyze(string + i + 3, len - 1); //)+(>123<)
    //             }
    //             return tmp_op;
    //         };
    //     }
    // }
    return new_op(string, len); //递归底层返回op //>123<
}

// double op_calc(op *unit)
// {
//     return unit->get_value(unit);
// }

void op_clean(op *unit)
{
    if (unit->child[0] != NULL)
    {
        op_clean(unit->child[0]);
    }
    if (unit->child[1] != NULL)
    {
        op_clean(unit->child[1]);
    }
    free(unit);
}

void op_print(op *unit)
{
    if (unit->data.symple[0] == '@')
    {
        Serial.print("op:");
        Serial.print((int)unit);
        Serial.print(" content:");
        Serial.println(unit->data.symple + 1);
    }
    else
    {
        Serial.print("op:");
        Serial.print((int)unit);
        Serial.print(" content:");
        Serial.println(unit->data.number);
    }
    if (unit->child[0] != NULL)
    {
        op_print(unit->child[0]);
    }
    if (unit->child[1] != NULL)
    {
        op_print(unit->child[1]);
    }
}
static size_t __get_free_mem2(size_t start, size_t end)
{
    unsigned char *p;
    while (start < end - 1)
    {
        size_t size = (start + end) / 2;
        if (size == 0)
            return 0;
        p = (unsigned char *)malloc(size);
        if (p != NULL)
        { // malloc succeeded
            free(p);
            start = size;
        }
        else
        { // malloc failed
            end = size - 1;
        }
    }
    return start;
}
void setup()
{
    Serial.begin(9600);
    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, HIGH);
}

void loop()
{
    delay(1000);
    Serial.println("\nExpression Input: ");
    while (!Serial.available())
    {
        digitalWrite(PC13, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(100);               // wait for a second
        digitalWrite(PC13, LOW);  // turn the LED off by making the voltage LOW
        delay(200);
    }
    char *tmp = (char *)malloc(Serial.available() * sizeof(char));
    Serial.readBytes(tmp, Serial.available()); //1+5-8*7/9*(88*7-9*8)
    Serial.print("Received: ");
    Serial.print(tmp);
    Serial.print("len:");
    Serial.println(strlen(tmp));
    op *final = analyze(tmp, strlen(tmp)); //be->ed
    op_print(final);
    Serial.print("\nResult: ");
    Serial.println(final->get_value(final));
    //Serial.print(" Answer: ");
    //Serial.println((3.14) + ((1.414) * ((1.427) + (0.333))));
    // // Serial.print("\nSize of Data: ");
    // // Serial.println(sizeof(Data));
    op_clean(final);
    double tmp_num;
    // for (int i = 0; i < 5000; ++i)
    // {
    //     op *final = analyze(tmp, strlen(tmp) - 1);
    //     tmp_num = final->get_value(final);
    //     // Serial.println("Done!");
    //     //op_print(final);
    //     op_clean(final);
    // }

    Serial.println("Done!");
    Serial.println(__get_free_mem2(0x20000000, 0x20000010));
    free(tmp);
    delay(1000);
}