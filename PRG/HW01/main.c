#include <stdio.h>
#include <stdlib.h>

/* Error numbers for returns */
#define INPUT_ERROR 100
#define INTERVAL_ERROR 101
#define ODD_WIDTH_ERROR 102
#define FENCE_ERROR 103
#define NO_ERROR 0

/* Other constants */
#define SCAN_SUCCES1 1
#define SCAN_SUCCES2 2
#define FENCE_ENDS 1
#define FENCE_MIDDLE 0

/* Dimension limits (including both end points) */
#define MIN 3
#define MAX 69

/* Used functions */

int load_dimensions(int *w_p, int *h_p);
int load_fence(int *f_p, int height);
void print_house(int width, int height);
void print_house_w_fence(int width, int height, int fence);
void print_empty_line(int width);
void print_full_line(int width);
int roof_height(int width);
void print_roof(int roof_height);
void print_filled_line(int width, int offset);
void print_fence_line(int f_width, int start);

int main(int argc, char *argv[])
{
    int width, height, fence;
    int err;

    err = load_dimensions(&width, &height);

    if (err == NO_ERROR)
    {
        if (width == height)
        {
            err = load_fence(&fence, height);
            if (err == NO_ERROR)
            {
                print_house_w_fence(width, height, fence);
            }
        }
        else
        {
            print_house(width, height);
        }
    }

    return err;
}

int roof_height(int width)
{
    return (int)((width - 1) / 2);
}

void print_house_w_fence(int width, int height, int fence)
{
    int roof_h = roof_height(width);
    int fence_start = height - fence - 1;
    print_roof(roof_h);
    print_full_line(width);
    printf("\n");
    for (int i = 0; i < height - 2; ++i)
    {
        print_filled_line(width, i);
        if (i == fence_start)
        {
            print_fence_line(fence, FENCE_ENDS);
        }
        else if (i > fence_start)
        {
            print_fence_line(fence, FENCE_MIDDLE);
        }
        printf("\n");
    }
    print_full_line(width);
    print_fence_line(fence, FENCE_ENDS);
    printf("\n");
}

void print_fence_line(int f_width, int start)
{
    for (int i = 0; i < f_width; ++i)
    {
        if ((f_width - i) % 2 == 0)
        {
            if (start)
            {
                printf("-");
            }
            else
            {
                printf(" ");
            }
        }
        else
        {
            printf("|");
        }
    }
}

void print_house(int width, int height)
{
    int roof_h = roof_height(width);
    print_roof(roof_h);
    print_full_line(width);
    printf("\n");
    for (int i = 0; i < height - 2; ++i)
    {
        print_empty_line(width);
        printf("\n");
    }
    print_full_line(width);
    printf("\n");
}

void print_empty_line(int width)
{
    printf("X");
    for (int i = 0; i < width - 2; ++i)
    {
        printf(" ");
    }
    printf("X");
}

void print_full_line(int width)
{
    for (int i = 0; i < width; ++i)
    {
        printf("X");
    }
}
void print_filled_line(int width, int offset)
{
    printf("X");
    for (int i = 0; i < width - 2; ++i)
    {
        if ((offset + i) % 2 == 0)
        {
            printf("o");
        }
        else
        {
            printf("*");
        }
    }
    printf("X");
}
void print_roof(int roof_h)
{
    int mid_point = roof_h;

    for (int i = 0; i < roof_h; ++i)
    {
        for (int j = 0; j < roof_h + i + 1; ++j)
        {
            if (j == mid_point - i || j == mid_point + i)
            {
                printf("X");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int load_fence(int *f_p, int height)
{
    int err = NO_ERROR;

    if (scanf("%d", f_p) != SCAN_SUCCES1)
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        err = INPUT_ERROR;
    }
    else
    {
        if ((*f_p < 1) || (*f_p >= height))
        {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            err = FENCE_ERROR;
        }
    }
    return err;
}

int load_dimensions(int *w_p, int *h_p)
{
    int err = NO_ERROR;

    if (scanf("%d%d", w_p, h_p) != SCAN_SUCCES2)
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        err = INPUT_ERROR;
    }
    else
    {

        if ((*w_p < MIN) || (*w_p > MAX) || (*h_p < MIN) || (*h_p > MAX))
        {
            fprintf(stderr, "Error: Vstup mimo interval!\n");
            err = INTERVAL_ERROR;
        }
        else if (*w_p % 2 != 1)
        {
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            err = ODD_WIDTH_ERROR;
        }
    }
    return err;
}
