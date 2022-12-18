// The below program runs fine *IF* NDEBUG is defined
//# define NDEBUG
# include <assert.h>

int main()
{
    int x = 7;
    assert(false);
    assert (x==5);
    return 0;
}
