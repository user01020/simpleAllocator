#include <cstdio>
#include <cstdlib>
#include <ctime>	
#include <cstring>
#include "hexdump.h"
#include <list>
	
	
	/* struct BlockNode{
		
	}; */
	
	
	
	
//blockMetadata:
	
	/* struct Base{
		bool isBlockHead;
		bool isFree;
		size_t size;
	}; */
	
	struct BlockNode{
		struct FreeBlock *prev;
		struct FreeBlock *next;
		size_t size;
	};
	
	struct FreeBlockMetadata{
		bool isBlockHead;
		bool isFree;
		struct Node node;
	};
	
	struct AlockBlockMetadata{
		bool isBlockHead;
		bool isFree;
		size_t size;
	};
	
	struct BlockInfo{
		struct Node node;
		size_t size;
	};
	
	struct BlockMetadata{
		bool isBlockHead;
		bool isFree;
		union BlockInfo blockInfo;
	};
	
	
	typedef Metadata struct BlockMetadata
	
	
	
	typedef Node struct BlockNode
	
	Node *head = NULL;
	
	size_t getBlockMetadataSize(const Metadata *m){
		static const size_t freeBlock = sizeof(FreeBlockMetadata);
		static const size_t allocBlock = sizeof(AlockBlockMetadata);
		return m->isFree ? freeBlock : allocBlock;
	}
	
	size_t getSize(const Metadata *m){
		return m->isFree ? m->blockInfo.node.size : m->blockInfo.size;
	}
	
	size_t getBlockSize(const Metadata *m){
		/* uint8_t *startBlock = (uint8_t *)block;
		uint8_t *endBlock = (uint8_t *)block->borderTag.other + sizeof(BorderTag) - 1; */
		return getBlockMetadataSize(m) * 2 + getSize(m);
	}
	
	
	Metadata *getOtherMetadata(const Metadata *m){
		size_t dist = getBlockMetadataSize(m) + getSize(m);
		return m->isBlockHead ? m + dist : m - dist;
	}
	
	/* void syncMetadata(const Metadata* block){
		Metadata *other = getOtherMetadata(m);
		memcpy(other, m, getBlockMetadataSize(m));
		//other->isBlockHead = m->isBlockHead ^ 1;
		other->isBlockHead = m->isBlockHead ? false : true;
	} */
	
	/* void setBlockMetadata(Metadata *block, const Metadata *m){
		
	} */
	
	void setSize(Metadata *block, size){
		
	}
	
	
	void decrease(Metadata *m, size_t size){
		if(!m->isFree) throw 1;
		
		
		
		
		FreeBlock *head, newTail;
		size_t oldSize, newSize;
		head = block;
		newSize = head->size - size;
		//oldTail = head + sizeof(FreeBlock) + oldSize;
		newTail = head + sizeof(FreeBlock) + newSize;
		head->size = newSize;
		*newTail = *head;
	}
	
	
	
	void addToList(FreeBlock *block){
		if(head == NULL){
			block->prev = head;
			block->next = head;
			return;
		}
		FreeBlock *tail = block->prev;
		block->prev = tail;
		block->next = head;
		tail->next = block;
		head->prev = block;
	}

	void removeFromList(freeBlock *block){
		if(head == NULL) return;
		if(head->next == head) return;
		FreeBlock *prev, *next;
		prev = block->prev;
		next = block->next;
		prev->next = next;
		next->prev = prev;
	}
	
	void allocateBlock(Block *block){
		AllocBlock *block = (AllocBlock *)block;	
		
	}
	
	
	void freeBlock(allocBlock *block){
		Block minFreeBlock = {true, 0}
		if(getBlockSize(block) < getBlockSize(&minFreeBlock))
			throw 1;
		block->base.isFree = false;
	}
	
	
	
	
	
	
	//#pragma pack(pop)

	/* size_t blockMetadataSize(){
		Block b;
		return b.mem - (uint8_t *)&b;
	} */
	
	//const size_t BLOCKMETADATASIZE = blockMetadataSize();
	//#define BLOCKMETADATASIZE (sizeof(Block) - 1)
	#define BLOCKSIZE(block) (block->size + BLOCKMETADATASIZE)
	
	/* void *mem = NULL;
	size_t memSize = 0; */
	
	
	//std::list<Block *> blocks;
	
	
	addToList(FreeBlock *block){
		if(head == NULL){
			
		}
	}
	
	
	
	
	/* Block *createBlock(bool isFreeBlock, void *buf, size_t blockSize){
	    
		//printf("createBlock:\n");
		//printf("buf = %p\n", buf);
		//printf("blockSize = %d\n", blockSize);
		Block *block = (Block *)buf;
		//printf("block p = %p\n", block);
		block->isFreeBlock = isFreeBlock;
		block->size = blockSize - BLOCKMETADATASIZE;
		//printf("block->size = %d\n", block->size);
		if(isFreeBlock){
			if(head == NULL){
				head = block;
				//printf("head = %p\n", head);
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
		
		//debug
		block->mem[block->size - 1] = 2;
		blocks.push_back(block);
		//
		return block;
	} */
	
	/* void deleteBlock(Block *block){

		printf("deleteBlock\n");

		for(auto i = blocks.begin(); i != blocks.end(); ++i){
			if(*i == block) blocks.erase(i);
		}
		
		if(isFreeBlock(block)){
			removeFromList(block);
		} else {
			Block *rightBlock = getRightBlock(block);
			Block *leftBlock = getLeftBlock(block);
			if(isFreeBlock(rightBlock)){
				expand(block, leftBlock);
			}
			if(isFreeBlock(leftBlock)){
				block = expand(block, leftBlock);
			}
			freeMem(block);
		}
	} */
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
		//head = NULL;
		printf("mysetup:\n");
		//printf("buf = %p\n", buf);
		//printf("size = %d\n", size);
		//printf("head = %p\n", head);
		//mem = buf;
		//memSize = size;
		//Block *block = (Block *)buf;
		//printf("BLOCKSIZE(block) = %d\n", BLOCKSIZE(block));
		//printf("head = %p\n", head);
		
		initAllocator();
    }
	
	
    // Функция аллокации
	void *myalloc(std::size_t size)
	{
		printf("myalloc:\n");
		//if(head == NULL) return NULL;
		//printf("head = %p\n", head);
		/* Block *freeBlock = head;
		//printf("freeBlock = %p\n", freeBlock);
		//*freeBlock;
		Block *allocBlock;
		size_t allocBlockSize = size + BLOCKMETADATASIZE; */
		
		FreeBlock *block = findFreeBlock(size);
		
		if(block == NULL) return NULL;
		
		allocMem(block, size);
		/*
		do{
			
			if(freeBlock->size >= allocBlockSize){
				freeBlock->size -= allocBlockSize;
				void *buf = freeBlock->mem + freeBlock->size;
				allocBlock = createBlock(false, buf, allocBlockSize);
				//allocBlock->prev = freeBlock;
				//uint8_t *memEnd = (uint8_t *)mem + memSize - 1;
				//uint8_t *allocBlockEnd = allocBlock->mem + allocBlock->size - 1;
				//if(allocBlockEnd < memEnd){
					//allocBlock->next = (Block *)(allocBlock->mem + allocBlock->size);
					//Block *rightBlock = allocBlock->next;
					//if(!rightBlock->isFreeBlock)
						//rightBlock->prev = allocBlock;
				}
				//->size + BLOCKMETADATASIZE;
				return allocBlock->mem;
			}
			freeBlock = freeBlock->next;
		} while(freeBlock != head);
		return NULL;*/
	}

	
    // Функция освобождения
	void myfree(void *p)
	{
	    printf("myfree\n");
		//Block *block = (Block *)((uint8_t *)p - BLOCKMETADATASIZE);
		auto it = blocks.begin();
		int n = blocks.size();
		for(int i = 0; i < n; i++, it++){
			if(*it == block) {
				blocks.erase(it);
			}
		}
		printf("block = %p\n", block);/*
		Block *leftBlock = block->prev;
		Block *rightBlock = block->next;
		if(rightBlock != NULL) 
			if(rightBlock->isFreeBlock){
				block->size += BLOCKSIZE(rightBlock);//->size + BLOCKMETADATASIZE;
				deleteBlock(rightBlock);
			} else {
				rightBlock->prev = block->prev;
			}
		if(leftBlock != NULL) 
			if(leftBlock->isFreeBlock){
				leftBlock->size += BLOCKSIZE(block);
				return;
			} else {
				leftBlock->next = block->next;
			}*/
		deleteBlock(getBlock(p));
	}
////////////////////////////////////////////

		// каждая сточка по 8 байт
		// в каждой сточке : адресс: 
	void printBlocks(){
		//static const width = 16;
		static char devider[35] = { 0 };
		if(devider[0] != '.') memset(devider, '.', 34);
		blocks.sort();
		for(auto block : blocks){
			hexdump(block, sizeof(Block), 8, false);
			char str[35] = { 0 };
			printf("\n\n%s\n", devider);
			hexdump((uint8_t *)block + BLOCKSIZE(block) - 1, 1, 8, false);
			printf("\n\n%s\n", devider);
		}		
	}
	
	//template <class type>
	bool mixList(const void * a, const void * b){
		return rand() % 2;
	}
	
	void testFree(std::list<void *> &addrs){
		printf("free\n");
		auto i = addrs.end();
		i--;
		void *addr = *i;
		addrs.pop_back();
		printf("addr = %p\n", addr);
		myfree(addr);
	}
	
	void testAlloc(std::list<void *> &addrs, size_t maxSize){
		printf("alloc\n");
		size_t allocSize = rand() % (maxSize + 1);
		//printf("allocSize=%d\n", allocSize);
		void *addr = myalloc(allocSize);
		//printf("addr = %p\n", addr);
		if(addr == NULL) testFree(addrs);
		else addrs.push_back(addr);
		if(addrs.size() > 1)
		addrs.sort(mixList);
	}
	
	
	void test(size_t size){
	    printf("\nstart testing\n");
		std::list<void *> addrs;
		testAlloc(addrs, size);
		printBlocks();
		for(int i = 0; i < 10; i++){
			printf("test %d:\n", i);
			bool alloc = rand() % 2;
			if(alloc || addrs.empty()){
				testAlloc(addrs, size);
			} else {
				testFree(addrs);
			}
			printBlocks();
			printf("addrs:\n");
			for(auto addr : addrs) {
				printf("%p\n", addr);
			}
		}
	}
/*
	struct aa{
		int i;
		short s;
		short s2;
		char c;
	};*/

int main(){
	srand(time(NULL));
	//struct aa s = {~(int)0 - 1, ~(short)0 - 1, ~(short)0, ~(char)0};
	
	//printf("&s = %p\n", &s);
	//printf("sizeof(s) = %d", sizeof(s));
	
	//hexdump(&s, sizeof(s));
	//printf("\n\n");
	
	size_t minSize = 1 << 10;
	size_t maxSize = minSize << 10;
	printf("minSize = %d, maxSize = %d\n", minSize, maxSize);
	//printf("RAND_MAX = %d\n", RAND_MAX);
	size_t size = (rand() + minSize) % (maxSize + 1);
	printf("mem size = %d\n", size);
	
	uint8_t *mem = (uint8_t *)malloc(size);
	//hexdump(mem, 1);
	printf("mem = [ %p - %p ]\n", mem, mem + size - 1);
	if(mem == NULL){
		printf("error get memory!\n");
		return 1;
	}
	mysetup(mem, size);
	printBlocks();
	test(size);
	return 0;
}
