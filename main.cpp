#include <cstdio>
#include <cstdlib>
#include <ctime>	
#include <cstring>
#include "hexdump.h"
#include <list>

using namespace std;
	
//all structs point in one part of mem and start with same addr
	/*
	struct _AllocBlockMetadata{
		bool isFree;
		size_t size;
	};*/
	
	struct _BlockMetadata{
	    bool isFree; //если блок свободный
	    bool isHead; //если это метаданные в начале блока
	    size_t size;
	    struct _BlockMetadata *prev;
	    struct _BlockMetadata *next;
	};
	
	//typedef BlockI struct _BlockInfo;
	typedef uint8_t Byte;
	typedef struct _BlockMetadata Node;
	typedef struct _BlockMetadata BlockMetadata;
	typedef struct _BlockMetadata AllocBlockMetadata;
	typedef struct _BlockMetadata FreeBlockMetadata;
	//typedef PBlock BlockMetadata *;
	Node *head = NULL;
    
	BlockMetadata *getHeadMetadataPoint(void *block){
	    return (BlockMetadata *)block;
	}
	
	BlockMetadata &getMetadata(void *block){
	    return *getHeadMetadataPoint(block);
	}
	
	BlockMetadata *getMetadataPointFromNode(Node *node){
	    return (BlockMetadata *)node;
	}
	
	void *getMemPoint(void *block){
	    return (void *)((Byte *)block + sizeof(BlockMetadata));
	}
    
    void *getBlockPoint(void *blockMemPoint){
        return (void *)((Byte *)blockMemPoint - sizeof(BlockMetadata));
    }
    
    void *getBlockPointFromMetaPoint(BlockMetadata *pmetadata){
        if(pmetadata->isHead)
            return (void *)pmetadata;
        return (void *)((Byte *)pmetadata - pmetadata->size - sizeof(BlockMetadata));
    }
    
    void *getBlockPointFromNode(Node *node){
        BlockMetadata *pmetadata = getMetadataPointFromNode(node);
        return getBlockPointFromMetaPoint(pmetadata);
    }
	
	Node *getNodePoint(FreeBlockMetadata &metadata){
	    return (Node *)&metadata;
	}
	
	bool isFreeBlock(void *block){
	    return getMetadata(block).isFree;
	}
	
	size_t getMetadataSize(BlockMetadata &metadata){
	    return sizeof(metadata);
	}
	
	size_t getBlockMetadataSize(void *block){
		return getMetadataSize(getMetadata(block));
	}
	
	size_t getMemSize(void *block){
		return getMetadata(block).size;
	}
	
	size_t getBlockSizeFromMeta(BlockMetadata &metadata){
	    return getMetadataSize(metadata) *
	    2 + metadata.size;
	}
	
	size_t getBlockSize(void *block){
		return getBlockSizeFromMeta(getMetadata(block));
	}
	
	void *getRightBlock(void *block){
	    return (void *)((Byte *)block + getBlockSize(block));
	}
	
	void *getLeftBlock(void *block){
	    BlockMetadata &leftBlockMetadata = *(BlockMetadata *)((Byte *)block - sizeof(BlockMetadata));
	    return (void *)((Byte *)block - getBlockSizeFromMeta(leftBlockMetadata));
	}
///////////////////////////////////////	
	void *setMetadata(void *block, BlockMetadata &metadata){
	    printf("\n setMetadata\n");
	    printf("\n block = %p\n", block);
	    //bool isFreeBlock = metadata.isFree;
	    size_t memsize = metadata.size;
	    size_t metadatasize = getMetadataSize(metadata);
	    size_t blocksize = memsize + metadatasize * 2;
	    //
	    
	    printf("\n memsize = %zu\n metadatasize = %zu\n blocksize = %zu\n", memsize, metadatasize, blocksize);
	    //
	    //BlockMetadata *pmetadata = &metadata;
	    BlockMetadata *pheadmeta = (BlockMetadata *)block;
	    BlockMetadata *ptailmeta = (BlockMetadata *)((Byte *)pheadmeta + blocksize - metadatasize);
	    //
	    printf("\n pmetadata = %p\n pheadmeta = %p\n ptailmeta = %p\n",(void *)&metadata, (void *)pheadmeta, (void *)ptailmeta);
	    //
	    if(pheadmeta != &metadata){
	        //memcpy(pheadmeta, pmetadata, metadatasize);
	        *pheadmeta = metadata;
	    }
	    if(ptailmeta != &metadata){
	        //memcpy(ptailmeta, pmetadata, metadatasize);
	        *ptailmeta = metadata;
	    }
	    pheadmeta->isHead = true;
	    ptailmeta->isHead = false;
	    return block;
	}
	/*
	void setMemSize(void *block, size_t memsize){
	    BlockMetadata &metadata = getMetadata(block);
	    //if(memsize > metadata.size) return;
	    metadata.size = memsize;
	    setMetadata(block, metadata);
	}
	*/
//////////////////////////////////////
	void addToList(Node *node){
		if(head == NULL){
		    head = node;
			node->prev = head;
			node->next = head;
			return;
		}
		Node *tail = head->prev;
		node->prev = tail;
		node->next = head;
		tail->next = node;
		head->prev = node;
	}
	
	void removeFromList(Node *node){
		if(head == NULL) return;
		if(head->next == head) return;
		if(node == head) head = head->next;
		Node *prev, *next;
		prev = node->prev;
		next = node->next;
		prev->next = next;
		next->prev = prev;
	}
	
	void removeBlock(void *block);
//////////////////////////////////////	
		//second block merge to first block
	void *merge(void *firstblock, void *secondblock){
	    if(firstblock == secondblock) return NULL;
	    BlockMetadata &FBmetadata = getMetadata(firstblock);
	    BlockMetadata &SBmetadata = getMetadata(secondblock);
	    void *block = firstblock < secondblock ? firstblock : secondblock;
	    if(SBmetadata.isFree) 
	        removeBlock(secondblock);
	    //setMemSize(block, FBmetadata.size + SBmetadata.size);
	    FBmetadata.size += SBmetadata.size;
	    setMetadata(block, FBmetadata);
	    return block;
	}
	
	void *createBlock(void *blockpoint, bool isFreeBlock, size_t memsize){
	    //
	    printf("\n createBlock\n");
	    printf("\n memsize = %zu\n", memsize);
	    //
	    BlockMetadata *pheadmeta = getHeadMetadataPoint(blockpoint);
	    //
	    printf("\n pheadmeta = %p \n", pheadmeta);
	    //
	    pheadmeta->isFree = isFreeBlock;
	    pheadmeta->size = memsize;
	    //
	    //printf("\n metadata.size = %zu\n", metadata.size);
	    //
	    if(isFreeBlock){
	        FreeBlockMetadata &FBmetadata = *(FreeBlockMetadata *)pheadmeta;
	        addToList(getNodePoint(FBmetadata));
	    }
	    setMetadata(blockpoint, *pheadmeta);
	    return blockpoint;
	}
	
	void toFreeBlock(void *block){
	    if(isFreeBlock(block)) return;
	    BlockMetadata &metadata = getMetadata(block);
	    metadata.isFree = true;
	    addToList(getNodePoint((FreeBlockMetadata &)metadata));
	    setMetadata(block, metadata);
	}
	
	void removeBlock(void *block){
	    BlockMetadata &metadata = getMetadata(block);
	    if(metadata.isFree){
	        FreeBlockMetadata &FBmetadata = *(FreeBlockMetadata *)&metadata;
	        removeFromList(getNodePoint(FBmetadata));
	    } else {
	        void *rightblock = getRightBlock(block);
	        void *leftblock = getLeftBlock(block);
	        if(isFreeBlock(rightblock)){
	            merge(rightblock, block
	            );
	        }
	        if(isFreeBlock(leftblock)){
	            merge(leftblock, block);
	        }
	        //toFreeBlock(block)
	    }
	}

    //find free block what have size >= memsize
    void *findFreeBlock(size_t memsize){
        //
        printf("\n findFreeBlock\n");
        printf("\n head = %p\n", head);
        //
        if(head == NULL) return NULL;
        Node *node = head;
        do{
            //
            printf("\nDo node = %p\n", node);
            if(node->size >= memsize) 
            return getBlockPointFromNode(node);
            node = node->next;
        }while(node != head);
        return NULL;
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
	    size_t memsize = size - sizeof(BlockMetadata) * 2;
	    createBlock(buf, true, memsize);
	    //
	    printf("\n head = %p\n", head);
	    //
    }
	
	
    // Функция аллокации
	void *myalloc(std::size_t size)
	{
		printf("myalloc:\n");
		//if(head == NULL) return NULL;
		//printf("head = %p\n", head);
		/* Block *freeBlock = head;
		//printf("freeBlock = %p\n", freeBlock);
	;
		Block *allocBlock;
		size_t allocBlockSize = size + BLOCKMETADATASIZE; */
		size_t blocksize = size + sizeof(AllocBlockMetadata) * 2;
		void *freeblock = findFreeBlock(blocksize);
		
		if(freeblock == NULL) return NULL;
		//
		printf("\n freeblock = %p\n blocksize = %zu\n freeblockmemsize = %zu\n", freeblock, blocksize, getMemSize(freeblock));
		//
		BlockMetadata &metadata = getMetadata(freeblock);
		metadata.size -= blocksize;
		setMetadata(freeblock, metadata);
		void *allocblock = (void *)((Byte *)freeblock + getBlockSize(freeblock));
		createBlock(allocblock, false, size);
		return getMemPoint(allocblock);
	}

	
    // Функция освобождения
	void myfree(void *p)
	{
	    printf("myfree\n");
		
		//printf("block = %p\n", block);
		void *block = getBlockPoint(p);
		removeBlock(block);
	}
////////////////////////////////////////////

		// каждая сточка по 8 байт
		// в каждой сточке : адресс: 
		/*
	void printBlocks(){
		//static const width = 16;
		static char devider[35] = { 0 };
		if(devider[0] != '.') memset(devider, '.', 34);
		blocks.sort();
		for(auto block : blocks){
			hexdump(block, sizeof(BlockMetadata), 8, false);
			char str[35] = { 0 };
			printf("\n\n%s\n", devider);
			hexdump((uint8_t *)block + BLOCKSIZE(block) - 1, 1, 8, false);
			printf("\n\n%s\n", devider);
		}		
	}
	*/
	//template <class type>
	bool mixList(const void * a, const void * b){
		return rand() % 2;
	}
	
	void testFree(std::list<void *> &addrs){
		//printf("free\n");
		
		auto i = addrs.end();
		i--;
		void *addr = *i;
		addrs.pop_back();
		//printf("addr = %p\n", addr);
		myfree(addr);
	}
	
	void testAlloc(std::list<void *> &addrs, size_t maxSize){
		//printf("alloc\n");
		size_t allocSize = rand() % (maxSize + 1);
		//printf("allocSize=%d\n", allocSize);
		void *addr = myalloc(allocSize);
		//printf("addr = %p\n", addr);
		if(addr != NULL) 
		    addrs.push_back(addr);
		if(addrs.size() > 1)
		addrs.sort(mixList);
	}
	
	
	void test(size_t size){
	    printf("\nstart testing\n");
		std::list<void *> addrs;
		testAlloc(addrs, size);
		//printBlocks();
		for(int i = 0; i < 100; i++){
			//printf("test %d:\n", i);
			bool alloc = rand() % 2;
			if(alloc || addrs.empty()){
				testAlloc(addrs, size);
			} else {
				testFree(addrs);
			}
			//printBlocks();
			//printf("addrs:\n");
			/*
			for(auto addr : addrs) {
				printf("%p\n", addr);
			}
			*/
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
	//printBlocks();
	test(size);
	return 0;
}
