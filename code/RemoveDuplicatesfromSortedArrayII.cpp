int removeDuplicates(int a[], int n)
{
    if(n <= 2)
    {
        return n;
    }
    
    int index = 2;
    for(int i = 2; i < n; ++i)
    {
        if(a[i] != a[index - 2])
        {
            a[index++] = a[i];
        }
    }
    
    return index;
    
}