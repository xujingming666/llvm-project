void __DUMMY_TEST();

int gogo(int a, int b)
{
	__DUMMY_TEST();
	 if(a > 0)
	 	return a+b > 10 ? a : b;
	 else
	 	return a-b;
}




