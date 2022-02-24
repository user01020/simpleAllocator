void hexdump(void *buf, size_t size, size_t bytec = 8){
    uint8_t *startAddr = (uint8_t *)buf;
    uint8_t *addr = startAddr & ~(bytec - 1);
    const size_t alignMask = bytec - 1;
    /*
	uint8_t *alignFirst;
	uint8_t *alignLast;
	alignFirst = bytes & alignMask;
	alignLast = (bytes + size) & alignMask;
    alignAddr = alignFirst;
	size_t lines = (alignLast - alignFirst + bytec) / bytec;
	*/
	int byten;		// строка столбец байт
//	char c;			// символ разделяющий байты

    addr: 00 00 00 .. 00
    addr: 00 00 00 .. 00


    do {
        alignAddr = bytes & alignMask;
		printf("\n");
		printf("%p: ", alignAddr);
		for(byten = 0; (byten < bytec); byten++){
		    if()
			printf("%x", *bytes);
			printf(" ");
		}
	} while (true);
}