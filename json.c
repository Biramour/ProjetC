#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#include<stdbool.h>
#include</home/biramour/ProgC/Projet/json.h>
#define prl { printf("\n");}

//STRUCTURES

typedef struct json_list_s* json_list_t;

struct json_list_s{
	const char* key;
	json_element_t* value;
	json_list_t next;	
};


typedef enum{
	INTEGER,
	DOUBLE
}number_type_t;

struct json_element_s{
	union{
		void* null;
		bool boolean;
		long int Integer;
		double Double;
		char* string;
		json_element_t* *array;
		json_list_t list;
	};
	number_type_t number_type;
	json_type_t type;
	int array_length;
};


// NULL
json_type_t json_type(const json_element_t* e){
	if(e==NULL) return JSON_NULL;
	return (*e).type;
}


//FREE
void json_free(json_element_t* e){
	json_type_t type = json_type(e);
	json_list_t c =NULL;
	int i;
	switch(type){
		case JSON_NULL:free(e); break;
		case JSON_BOOLEAN: free(e); break;
		case JSON_NUMBER: free(e); break;
		case JSON_STRING: free(e->string);free(e); break;
		case JSON_OBJECT: c=(*e).list; 
		while(c!=NULL){
			json_free(c->value);
			c = c->next;
		}free(e); break;
		case JSON_ARRAY: free((*e).array); free(e); break;
	}
}

//BOOL
json_element_t* json_boolean(bool b){
	json_element_t* e=malloc(sizeof(json_element_t*)); 
	(*e).type=JSON_BOOLEAN;
	(*e).boolean = b;
	return e;
	json_free(e);
}

bool json_as_boolean(const json_element_t* e){
	if(json_type(e)!=JSON_BOOLEAN){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	} else{
		return (*e).boolean;
	}
}

//NUMBER
json_element_t* json_integer(long i){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_NUMBER;
	(*e).Integer=i;
	(*e).number_type=INTEGER;
	return e;
	json_free(e);
}

json_element_t* json_double(double d){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_NUMBER;
	(*e).Double=d;
	(*e).number_type=DOUBLE;
	return e;
	json_free(e);
}

bool json_is_integer(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER || e->number_type!=INTEGER){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	}else{
		return true;
	}
}

bool json_is_double(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER || e->number_type!=DOUBLE){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	}else{
		return true;
	}
}

long json_as_integer(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	}else if((*e).number_type==DOUBLE){
		return (long)((*e).Double);
	}else return (*e).Integer;
}

double json_as_double(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	}else if((*e).number_type==INTEGER){
		return (float)((*e).Integer);
	}else return (*e).Double;
}

//STRING
json_element_t* json_string(const char* s){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_STRING;
	(*e).string=malloc(strlen(s)+1);
	strcpy((*e).string,s);
	return e;
	json_free(e);
}

const char* json_as_string(const json_element_t* e){
	if(json_type(e)!=JSON_STRING){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	} else{
		return (*e).string;
	}
}	

//ARRAY
json_element_t* json_array(){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_ARRAY;
	(*e).array_length = 0;
	return e;
	json_free(e);
}

json_element_t* json_array_get(const json_element_t* e,size_t i){
	if(json_type(e)!=JSON_ARRAY){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	} else if((int)i < (*e).array_length){
		return (*e).array[i];
	} else if ((int)i >= (*e).array_length){
		return json_null();
	}
}

void json_array_set(json_element_t* e,size_t i,json_element_t* se){
	if(json_type(e)!=JSON_ARRAY){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	} else if ((*e).array_length == 0 && i==0){
		(*e).array = realloc ((*e).array,sizeof(json_element_t*));
		(*e).array[i]=se;
		(*e).array_length = 1;
	} else if((int)i <= (*e).array_length){
		(*e).array[i] = se;
	} else if ((int)i > (*e).array_length){
		(*e).array = realloc((*e).array, sizeof(json_element_t*)*((*e).array_length+i));
		for(int j = (*e).array_length;j<(int)i; j ++){
			(*e).array[j]=json_null();
		}
		(*e).array[i]=se;
		(*e).array_length = i+1;
	}	
}

//OBJECT
json_element_t* json_object(){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_OBJECT;
	(*e).list = NULL;
	return e;
}


json_element_t* json_object_get(const json_element_t* e, const char* key){
	if(json_type(e)!=JSON_OBJECT){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	} else {
		json_list_t c = e->list;
		while(c !=NULL){
			if(strcmp(c->key,key) == 0){
				return (*c).value;
			} else c = (*c).next;
		}
		return json_null();
	} 
}

void json_object_set( json_element_t* e, const char* key, json_element_t* value){
	if(json_type(e)!=JSON_OBJECT){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	} else {
		json_list_t c = e->list;
		while(c !=NULL){
			if(strcmp(c->key,key) == 0){
				json_free((*c).value);
				(*c).value = value;
				return; 
			} else c = (*c).next;
		}

		char* a = malloc(strlen(key)+1);
		strcpy(a,key);
		json_list_t new = malloc(sizeof(struct json_list_s));
		new->key = a;
		new->value = value;
		new->next = e->list;
		e->list = new;
	
	} 
}


//ENTREES SORTIES
void json_put(FILE* fd, const json_element_t* e){
	json_type_t type = json_type(e);
	json_list_t c =NULL;
	int i;
	switch(type){
		case JSON_NULL:fprintf(fd,"null");break;
		case JSON_BOOLEAN: fprintf(fd,"%s",json_as_boolean(e)?"true":"false"); break;
		case JSON_NUMBER: switch((*e).number_type){	
			case INTEGER: fprintf(fd,"%ld",json_as_integer(e)); break;
			case DOUBLE: fprintf(fd,"%.8f",json_as_double(e)); break;	} break;
		case JSON_STRING: fprintf(fd,"\"%s\"",json_as_string(e)); break;
		case JSON_ARRAY: fprintf(fd,"[");
		for(i = 0; i<(*e).array_length;i++){
			json_put(fd,(*e).array[i]);
			if(i != (*e).array_length-1){
				fprintf(fd,",");
			}
		}
		fprintf(fd,"]");break;
		case JSON_OBJECT:fprintf(fd,"{"); 
		c=e->list; 
		while(c!=NULL){
			if(c->next == NULL){
				fprintf(fd,"\"%s\":",(*c).key);
				json_put(fd,(*c).value);
				break;
			}
			fprintf(fd,"\"%s\":",(*c).key);
			json_put(fd,(*c).value);
			fprintf(fd,",");
			c = (*c).next;
			}
			fprintf(fd,"}");
	}
}

void json_save(const char* fname,json_element_t* e){
	FILE* fd = fopen(fname,"w");
	json_put(fd,e);
}

//IMPORT
char next_char(FILE* fd){
	char c = fgetc(fd);
	while(c==' ' || c=='\n' || c=='\r' || c=='\t'){
		c = fgetc(fd);
	}
	if(c==EOF) exit(EXIT_FAILURE);
	else return c;
}

void check_next_char(FILE* fd, char c){
	char a = next_char(fd);
	if(a==c) exit(EXIT_SUCCESS);
	else exit(EXIT_FAILURE);
}

bool is_next_char(FILE* fd,char c, bool cons){
	char a = next_char(fd);
	if(a==c && cons==true){
		return true;
	}
	else{
		ungetc(a,fd);
		return false;
	}
}

void check_next_word(FILE* fd, const char* w){
	char* c = malloc(strlen(w)+1);
	for(long unsigned int i =0;i<strlen(w);i++){
		c[i]=next_char(fd);
	}
	if(strcmp(c,w)==0) exit(EXIT_SUCCESS);
	else exit(EXIT_FAILURE);
}

char* next_string(FILE* fd){
	char a = next_char(fd);
	if(a!='\"') exit(EXIT_FAILURE);
	a = fgetc(fd);
	char* c = malloc(1*sizeof(char));
	c[0]='\0';
	int compteur = 0;
	while(a!='\"'){
		if(a==EOF) exit(EXIT_FAILURE);
		c = realloc(c,(compteur+2)*(sizeof(char)));
		c[compteur]=a;
		c[compteur+1]='\0';
		compteur++;
		a = fgetc(fd);
	}
	return c;
	free(c);
}

char* next_digits(FILE* fd){
	char a = next_char(fd);
	if(a<'0'|| a>'9') exit(EXIT_FAILURE);
	char* c = malloc(1*sizeof(char));
	c[0]='\0';
	int compteur = 0;
	while(a>='0' && a<='9'){
		c = realloc(c,(compteur+2)*sizeof(char));
		c[compteur]=a;
		c[compteur+1]='\0';
		compteur++;
		a=fgetc(fd);
	}
	ungetc(a,fd);
	return c;
	free(c);
}

json_element_t* json_get(FILE* fd) {
  int c = next_char(fd);
  if (c == 'n') {
    ungetc(c,fd);
    check_next_word(fd,"null");
    return json_null();
  }
  if (c == 't') {
    ungetc(c,fd);
    check_next_word(fd,"true");
    return json_boolean(true);
  }
  if (c == 'f') {
    ungetc(c,fd);
    check_next_word(fd,"false");
    return json_boolean(false);
  }
  if (c == '-') {
    json_element_t* se = json_get(fd);
    if (json_type(se) != JSON_NUMBER) {
      fprintf(stderr, "A number is expected after character '-'\n");
      exit(EXIT_FAILURE);
    }
    if (json_is_double(se)) {
      double d = json_as_double(se);
      json_free(se);
      return json_double(-d);
    }
    if (json_is_integer(se)) {
      long i = json_as_integer(se);
      json_free(se);
      return json_integer(-i);
    }
  }
  if ('0' <= c && c <= '9') {
    ungetc(c,fd);
    char* number = next_digits(fd);
    c = fgetc(fd);
    if (c == '.') {
      char* decimal = next_digits(fd);
      number = realloc(number, strlen(number) + strlen(decimal) + 2);
      strcat(number,".");
      strcat(number,decimal);
      json_element_t* e = json_double(strtod(number,NULL));
      free(number);
      free(decimal);
      return e;
    } else {
      ungetc(c,fd);
      json_element_t* e = json_integer(strtol(number,NULL,10));
      free(number);
      return e;
    }
  }
  if (c == '"') {
    ungetc(c,fd);
    char* str = next_string(fd);
    json_element_t* e = json_string(str);
    free(str);
    return e;
  }
  if (c == '[') {
    json_element_t* e = json_array();
    size_t i = 0;
    if (is_next_char(fd,']',true)) return e;
    while(1) {
      json_element_t* se = json_get(fd);
      json_array_set(e,i++,se);
      c = next_char(fd);

      if (c == ']') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted (character %ld)\n", c, ftell(fd));
        exit(EXIT_FAILURE);
      }
    }
  }
  if (c == '{') {
    json_element_t* e = json_object();
    if (is_next_char(fd,'}',true)) return e;
    while(1) {
      check_next_char(fd,'"');
      ungetc('"',fd);
      char* key = next_string(fd);
      check_next_char(fd,':');
      json_element_t* value = json_get(fd);
      json_object_set(e,key,value);
      c = next_char(fd);
      if (c == '}') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c' '}' or ',' excepted (character %ld)\n", c, ftell(fd));
        exit(EXIT_FAILURE);
      }
    }
  }

  fprintf(stderr, "Unexpected character '%c'; 'n', 't', 'f', [0-9], '\"', '[' or '{' excepted (character %ld)\n", c, ftell(fd));
  exit(EXIT_FAILURE);
}

json_element_t* json_load(const char* fname){
	FILE* fd = fopen(fname,"r");
	json_element_t* e = json_get(fd);
	fclose(fd);
	return e;
	json_free(e);
}


///////////////////////////////////////////////////////

int main() {
  json_element_t* e = NULL;
  e = json_null(); json_put(stdout, e); prl; json_free(e);
  e = json_boolean(true); json_put(stdout, e); prl; json_free(e);
  e = json_boolean(false); json_put(stdout, e); prl; json_free(e);
  e = json_integer(123456L); json_put(stdout, e); prl; json_free(e);
  e = json_double(123456.987654); json_put(stdout, e); prl; json_free(e);
  e = json_string("Un petit texte"); json_put(stdout, e); prl; json_free(e);
  e = json_array(); json_put(stdout, e); prl;
  json_array_set(e, 0, json_integer(1928374566L));
  json_array_set(e, 3, json_string("Un autre texte"));
  json_array_set(e, 1, json_boolean(false));
  json_put(stdout, e); prl; json_free(e);
  e = json_object(); json_put(stdout, e); prl;
  json_object_set(e, "key1", json_integer(1928374566L));
  json_object_set(e, "key2", json_string("Un autre texte"));
  json_object_set(e, "key3", json_boolean(false));
  json_object_set(e, "key2", json_string("Un texte de remplacement"));
  json_put(stdout, e); prl; json_free(e);
 
  FILE* fd = fopen("example.json","r");
  e = json_get(fd);
  fclose(fd);
  json_put(stdout,e);prl;
  json_free(e);

  return EXIT_SUCCESS;
}