void sendInFile(char fileName[])
{
    int n;
    n = open(fileName, O_RDONLY);

    dup2(n, 0);
    close(n);

    return;
}

void sendOutFile(char fileName[])
{
    int out;
    out = open(fileName, O_WRONLY);
    outCheck = 1;

    dup2(out, 1);
    close(out);

    return;
}

