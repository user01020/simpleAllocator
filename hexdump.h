void hexdump(void *buf, size_t size, size_t bytec = 8, bool isAlignOn = true){
    uint8_t *startByte = (uint8_t *)buf;
	size_t startByteAddr = (size_t)startByte;
	uint8_t *byte = isAlignOn ? (uint8_t *)(startByteAddr - (startByteAddr % bytec)) : startByte ;
   
	//addr: 00 00 00 .. 00
    //addr: 00 00 00 .. 00
	size_t i = 0;
	if(byte < startByte && isAlignOn){
		printf("\n");
		printf("%p:", byte);
		while(byte < startByte){
			printf("%3c", ' ');	
			i++;
			byte++;
		}
	}
		
    while (size--){
		if(i++ % bytec == 0){	
			printf("\n");		
			printf("%p:", byte);
		}
		printf("%3x", *byte++);
	}
}