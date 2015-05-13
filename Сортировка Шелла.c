// BaseType - любой перечисляемый тип 
// typedef int BaseType - пример
void ShellsSort(BaseType *A, unsigned N)
{
	unsigned i,j,k;
	BaseType t;
	for(k = N/2; k > 0; k /=2)
        for(i = k; i < N; i++)
        {
            t = A[i];
            for(j = i; j>=k; j-=k)
            {
                if(t < A[j-k])
                    A[j] = A[j-k];
                else
                    break;
            }
            A[j] = t;
        }
}