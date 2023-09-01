# Understanding the Mighty Tideman </br> <sub> A Noob's Humble Attempt</sub>

Need to Understand this Recursive Function

```c
bool cycle(int end, int start)
{
    if (end == start)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i])
            if (cycle(i, start))
            {
                return true;
            }
    }
    return false;
}

```