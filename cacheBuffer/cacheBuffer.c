#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100
#define T 5
#define L 7

struct NODE* printList(int inputType,struct NODE **head);
struct NODE* deleteList(struct NODE **head);
struct NODE* createNode(int* nodeCounter, char *address,struct NODE **head);

struct NODE{                         //Double linked list dugumlerinin olusturuldugu struct
	char *address;
	int counter;
	struct NODE *next;
	struct NODE *prev;
};

/*
@brief createNode fonksiyonu double linked list'e dugum ekleme, node icindeki counter T'den buyukse head node olarak atama 
ve node sayisi L'yi gecerse son node'u silme görevlerini yapar.
@param nodeCounter kac tane node olduðunu tutar. L ile karsilastirmak icin kullanilir
@param inputAddress kullanicidan alinan veya dosyadan okunan adresin atandigi degiskendir. Node icindeki address'e atanacak degeri tutar.
@param head head node'u tutar.
@return ustunde islem yapilan node'u dondurur.
*/
struct NODE* createNode(int* nodeCounter, char *inputAddress,struct NODE **head){
	int isNodeExist=0;
	struct NODE *newNode;
	newNode=(struct NODE*)malloc(sizeof(struct NODE));       //Dugume dynamic memort allocation ile yer acma
	if(newNode==NULL){
		printf("Allocation error.");
		exit(0);
	}
	 
	newNode->address=(char*)malloc(MAX*sizeof(char));        //Dugum icindeki address icin dynamic memort allocation ile yer acma
	if (newNode->address==NULL) {
        printf("Bellek ayrilamadi");
        return;
    }
    
	strcpy(newNode->address, inputAddress);       //Kullanicidan alinan veya dosyadan okunan adresi node icine kopyalama
	
	newNode->counter=1;                      //Node'un sayacýný arttýrma
	newNode->next=NULL;
	newNode->prev=NULL;
	
	if(*head==NULL){                          // Eger liste bossa node'u head node haline getirme           
		*head=newNode;
		(*nodeCounter)++;
	}
	else{
		struct NODE* current=*head;
		struct NODE* prevNode;
		struct NODE* nextNode;
		struct NODE* lastNode=*head;
		while(current!=NULL){                //Listeyi bastan sona dolasmak icin olusturulan dongu
			if(strcmp(current->address,newNode->address)==0){      //Listedeki adresle kullanicidan alinan veya dosyadan okunan adresin ayni olup olmadigi kontrolu
				current->counter=(current->counter)+1;             //Adres degeri ayni ise node icindeki counter bir artar
				isNodeExist=1;
				if((current->counter)>T){         //Node icindeki counter, esik degerden(T) buyukse o node'u head node yapma 
					prevNode=current->prev;
					nextNode=current->next;
					if(prevNode!=NULL){
						prevNode->next=nextNode;      //Head yapilacak node'un oncesindeki ve sonrasindaki node'lari birbirine baglama
					}
					if(nextNode!=NULL){
						nextNode->prev=prevNode;
					}
					current->prev=NULL;
					current->next=*head;          //Head node olarak atama
					(*head)->prev=current;
					*head=current;
				}
			}
			current=current->next;      //Tum dugumleri kontrol edebilmek icin sonraki dugume gecme
		}
		if(isNodeExist==0){                //Eger alinan adres onceden listede bulunmuyorsa yeni node olarak ekleme ve head node yapma
			newNode->next=*head;
			(*head)->prev=newNode;
			*head=newNode;
			(*nodeCounter)++;              //Node sayisinin sayacini arttirma
			if(*nodeCounter>L){           //Eger node sayisi Cache buffer’in kapasitesini(L) asiyorsa son node'u silme
				while(lastNode->next!=NULL){      //Listenin sonunu bulan dongu
					lastNode=lastNode->next;
				}
				lastNode->prev->next=NULL;
            	free(lastNode);
				(*nodeCounter)--;         //Node sayisinin sayacini azaltma
			}
		}	
	}
	return newNode;
}

/*
@brief printList fonksiyonu double linked list'i ekrana yazdirir. 
@param inputType adreslerin dosyadan mi yoksa elle girisle mi alindigi bilgisini tutar.
@param head head node'u tutar.
*/
struct NODE* printList(int inputType,struct NODE **head){  
	struct NODE *node;
	printf("Liste:\n");
	for(node=*head;node!=NULL;node=node->next){        //Listenin icinde gezme
		printf("Counter: %d  Address: %s",node->counter,node->address);
		if(inputType==2)
			printf("\n");
	}
	printf("\n\n");
}

/*
@brief deleteList fonksiyonu double linked list'i kullanicinin isteði dogrultusunda siler.
@param head head node'u tutar.
*/
struct NODE* deleteList(struct NODE **head){         
	struct NODE* node;
	struct NODE* nextNode;
	int deleteList=0;
	
	printf("Cache'i temizlemek icin 1 giriniz:  ");
	scanf("%d",&deleteList);
	
	if(deleteList==1){
		node=*head;
		while(node!=NULL){          //Listenin icinde gezerek dugumleri silen dongu
            nextNode=node->next; 
            free(node);
            node=nextNode; 
        }
        *head=NULL; 
		printf("Liste silindi.");
	}
}

int main(){
	FILE *file;
	struct NODE* head=NULL;
	char *inputAddress;       //Girilen adresi node icindeki address'e atanana kadar saklar
    char *fileName;
	int nodeCounter=0;        //Node sayisi
	int inputType;
	
    fileName = (char*)malloc(MAX * sizeof(char));    //fileName icin dynamic memory allocation ile yer ayýrma
    if (fileName == NULL) {
        printf("Bellek ayrilamadi.");
        return 1;
    }
	
	printf("Istekleri dosyadan okutmak icin 1\nElle girmek icin 2 giriniz.\nSeciminiz: ");     //Kullanicidan adresleri nasil girecegi secimini alma
	scanf("%d",&inputType);
	
	switch(inputType){
		case 1:               //Adres isteklerini dosyadan okuma durumu
			printf("Dosya ismi giriniz: ");
			scanf("%s",fileName);
			file= fopen(fileName,"r");
			if (file==NULL) {
        		printf("Dosya acilamadi");
       			return;
    		}
    		inputAddress=(char*)malloc(MAX*sizeof(char));         //address icin dynamic memory allocation ile yer ayýrma
    		if (inputAddress == NULL) {
    		    printf("Bellek ayrilamadi.");
       			return 1;
   			}
    
			while(fgets(inputAddress,MAX,file)!=NULL){    //Dosyayi satir satir okuyup address dizisine atama
				createNode(&nodeCounter,inputAddress,&head);               //Node olusturmak icin createNode fonksiyonunu cagirma
				printList(inputType,&head);                                //Listeyi yazdirma               
				inputAddress=(char*)malloc(MAX*sizeof(char));              //address icin dynamic memory allocation ile yer ayýrma
    			if (inputAddress == NULL) {  
    			    printf("Bellek ayrilamadi.");
        			return 1;
    			}
    
			}
			deleteList(&head);               //Adres istegi alimi sonlaninca kullaniciya cache’i temizlemek isteyip istemedigini soran fonksiyonu cagirma
			fclose(file);                    //Dosyayi kapama
			free(inputAddress);              //Dinamik bellekle olusturulan alanlari serbest birakma
			break;
			
		case 2:                                      //Adres isteklerinin elle girilmesi
			printf("Istek girisini sonlandirmak icin 0 giriniz.\n");    //Adresleri elle girme durumunda istek girisini sonlandirmak icin 0 girilmelidir
			do{
				inputAddress=(char*)malloc(MAX*sizeof(char));         //address icin dynamic memory allocation ile yer ayýrma
    			if (inputAddress == NULL) {
    			    printf("Bellek ayrilamadi.");
        			return 1;
    			}
    
				printf("Istek: ");
				scanf("%s",inputAddress);                 //Alinan adres istegi address dizisine atiliyor
				if (inputAddress[0]=='0'){                  //Eger 0 girilmisse node olusturma sonlanip liste yazdirilir
					printList(inputType,&head);
					deleteList(&head);
					free(inputAddress);             //Dinamik bellekle olusturulan alanlari serbest birakma
            		exit(0);
				}
				createNode(&nodeCounter,inputAddress,&head);     //0 girilmedigi durumda node olusturulur
				printList(inputType,&head);
			}while(inputAddress[0]!='0');              //Eger 0 girilmisse node olusturma sonlanip liste yazdirilir
			break;
			
	}
	
    free(fileName);                  //Dinamik bellekle olusturulan alanlari serbest birakma
}


