void main();

main(){
	int a, b, c;
	float d,e,f;
	a = (b<c) ? b:c;
	d = e<f?e:f;
	b = 1;
	c = 2;
	// b = 2;
	b = -b;

	if (b < c){
		a = c;
		b = c;
	}
	else{
		if (c == 1){
			a = b;
		}
		else if (b == 1){}
	}


}