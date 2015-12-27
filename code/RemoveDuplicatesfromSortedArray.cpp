size_t removeDuplicates(int a[], int n)
{
    if (n == 0)
    {
        return 0;
    }

    size_t index = 0;
    for (size_t i = 1; i < n; i++)
    {
        if (a[index] != a[i])
        {
            a[++index] = a[i];
        }
    }

    return index + 1;
}
