int main()
{
	float i;
	int j = 0x24, k = 024;
	i = j;
	while (i < 10) {	//使用float做比较
		i = i + 1;
		break;
	}
	break;	//错误用break
	
	for (k=0; k<5; k=k+1) {
		if (k < 3) 
			continue;
		else 
			j = j + 1;
	}
	
	continue;	//错用continue
}
