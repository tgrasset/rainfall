int main (void)
{
    for (int i = -1; i < 0; i = i - 1)
    {
        int r = i * 4;
        if (r > 0 && r < 50)
        {
            printf("%d       %d\n", r, i);
            return (1);
        }
    }
    return (0);
}