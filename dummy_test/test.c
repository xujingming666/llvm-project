void __DUMMY_TEST();

#if 0
int gogo(int a, int b)
{
	__DUMMY_TEST();
	 if(a > 0)
	 	return a+b > 10 ? a : b;
	 else
	 	return a-b;
}
#endif


int global;
int gogo1(int a, int b){
global++;

if (global < a){
	//return gogo(a,b);
	return global + a;
}

if (global < b){
	return global+b;
}

return a+b;
}
