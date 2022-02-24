#include <cstdio>
#include <cstdlib>
#include <ctime>	
#include <list>
	
	typedef struct _Block{
		bool isFreeBlock;			//если false, то next и prev соседние блоки
		struct _Block *next;			
		struct _Block *prev;
		size_t size;
		uint8_t mem[1];
	} Block;
	
	#define BLOCKMETADATASIZE (sizeof(Block) - 1)
	#define BLOCKSIZE(block) (block->size + BLOCKMETADATASIZE)
	Block *head;
	void *mem;
	size_t memSize;
	
	

	
	Block *createBlock(bool isFreeBlock, void *buf, size_t blockSize){
		printf("createBlock:\n");
		printf("buf = %p\n", buf);
		Block *block = (Block *)buf;
		printf("block p = %p\n", block);
		block->isFreeBlock = isFreeBlock;
		block->size = blockSize - BLOCKMETADATASIZE;
		if(isFreeBlock){
			if(head == NULL){
				head = block;
				printf("head = %p\n", head);
				block->next = head;
				block->prev = head;
			} else{
				Block *tail = head->prev;
				block->next = head;
				block->prev = tail;
				head->prev = block;
				tail->next = block;
			}
		} else {
			block->next = NULL;
			block->prev = NULL;
		}
		return block;
	}
	
	 // Эта функция будет вызвана перед тем как вызывать myalloc и myfree
    // используйте ее чтобы инициализировать ваш аллокатор перед началом
    // работы.
    //
    // buf - указатель на участок логической памяти, который ваш аллокатор
    //       должен распределять, все возвращаемые указатели должны быть
    //       либо равны NULL, либо быть из этого участка памяти
    // size - размер участка памяти, на который указывает buf
    
	void mysetup(void *buf, std::size_t size)
	{
		head = NULL;
		printf("mysetup:\n");
		printf("buf = %p\n", buf);
		printf("head = %p\n", head);
		mem = buf;
		memSize = size;
		createBlock(true, buf, size);
		printf("head = %p\n", head);
    }
	
	
    // Функция аллокации
	void *myalloc(std::size_t size)
	{
		printf("myalloc:\n");
		if(head == NULL) return NULL;
		printf("head = %p\n", head);
		Block *freeBlock = head;
		printf("freeBlock = %p\n", freeBlock);
		//*freeBlock;
		Block *allocBlock;
		size_t allocBlockSize = size + BLOCKMETADATASIZE;
		do{
			
			if(freeBlock->size >= allocBlockSize){
				freeBlock->size -= allocBlockSize;
				void *buf = freeBlock->mem + freeBlock->size;
				allocBlock = createBlock(false, buf, allocBlockSize);
				allocBlock->prev = freeBlock;
				uint8_t *memEnd = (uint8_t *)mem + memSize - 1;
				uint8_t *allocBlockEnd = allocBlock->mem + allocBlock->size - 1;
				if(allocBlockEnd < memEnd){
					allocBlock->next = (Block *)(freeBlock->mem + freeBlock->size);
				}
				//->size + BLOCKMETADATASIZE;
				return allocBlock->mem;
			}
			freeBlock = freeBlock->next;
		} while(freeBlock != head);
		return NULL;
	}

	void deleteBlock(Block *block){
		if(block->isFreeBlock){
			if(block == head){
				if(head->next == head) 
					return;
				head = head->next;
			}
			block->next->prev = block->prev;
			block->prev->next = block->next;
		} else {
			createBlock(true, block, BLOCKSIZE(block));
		}
	}
    // Функция освобождения
	void myfree(void *p)
	{
	    printf("myfree\n");
		Block *block = (Block *)((uint8_t *)p - BLOCKMETADATASIZE);
		Block *leftBlock = block->prev;
		Block *rightBlock = block->prev;
		if(rightBlock != NULL && rightBlock->isFreeBlock){
			block->size += BLOCKSIZE(rightBlock);//->size + BLOCKMETADATASIZE;
			deleteBlock(rightBlock);
		}
		if(leftBlock != NULL && leftBlock->isFreeBlock){
			leftBlock->size += block->size;
			return;
		}
		deleteBlock(block);
	}
////////////////////////////////////////////

		// каждая сточка по 8 байт
		// в каждой сточке : адресс: 
	void printMem(){
		
	}
	
	//template <class type>
	bool mixList(const void * a, const void * b){
		return rand() % 2;
	}
	
	void testFree(std::list<void *> &addrs){
		printf("free\n");
		void *addr = *addrs.end();
		addrs.pop_back();
		printf("addr = %p\n");
		myfree(addr);
	}
	
	void testAlloc(std::list<void *> &addrs, size_t maxSize){
		printf("alloc\n");
		size_t allocSize = rand() % (maxSize + 1);
		printf("allocSize=%d\n", allocSize);
		void *addr = myalloc(allocSize);
		printf("addr = %p\n", addr);
		if(addr == NULL) testFree(addrs);
		addrs.push_back(addr);
		addrs.sort(mixList);
	}
	
	
	void test(size_t size){
	    printf("\nstart testing\n");
		std::list<void *> addrs;
		testAlloc(addrs, size);
		for(int i = 0; i < 10; i++){
			printf("test %d:\n", i);
			bool alloc = rand() % 2;
			if(alloc || addrs.size() == 0){
				testAlloc(addrs, size);
			} else {
				testFree(addrs);
			}
		}
	}

int main(){
	//srand(time(NULL));
	size_t minSize = 1 << 10;
	size_t maxSize = minSize << 10;
	printf("minSize = %d, maxSize = %d\n", minSize, maxSize);
	printf("RAND_MAX = %d\n", RAND_MAX);
	size_t size = (rand() + minSize) % (maxSize + 1);
	printf("size = %d\n", size);
	
	void *mem = malloc(size);
	printf("mem point = %p\n", mem);
	if(mem == NULL){
		printf("error get memory!\n");
		return 1;
	}
	mysetup(mem, size);
	test(size);
	return 0;
}
