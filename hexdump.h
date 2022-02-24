void hexdump(void *buf, size_t size, size_t bytec = 8, bool isAlignOn = true){
    uint8_t *startByte = (uint8_t *)buf;
	//const size_t mask = bytec - 1;
    //uint8_t *byte = (uint8_t *)((size_t)startByte & ~(bytec - 1));
	size_t startByteAddr = (size_t)startByte;
	uint8_t *byte = isAlignOn ? (uint8_t *)(startByteAddr - (startByteAddr % bytec)) : startByte ;
    /*
	uint8_t *alignFirst;
	uint8_t *alignLast;
	alignFirst = bytes & alignMask;
	alignLast = (bytes + size) & alignMask;
    alignAddr = alignFirst;
	size_t lines = (alignLast - alignFirst + bytec) / bytec;
	*/
	//int byten;		// строка столбец байт
//	char c;			// символ разделяющий байты

   // addr: 00 00 00 .. 00
    //addr: 00 00 00 .. 00
	size_t i = 0;
	if(byte < startByte && isAlignOn){
		printf("\n");
		printf("%p:", byte);
		while(byte < startByte){//i =  1, 2, 3, 4
			printf("%3c", ' ');	  //byte = 04
			i++;
			byte++;
		}
	}
		
    while (size--){
		if(i++ % bytec == 0){	//i = 5, 6, 7, 8
			printf("\n");		//byte = 05, 06, 07, 08
			printf("%p:", byte);
		}
		printf("%3x", *byte++);
	}
}