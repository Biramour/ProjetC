#include <string.h>
#include <stdlib.h>
#include "json.h"


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
	json_type_t type;
	union{
	number_type_t number_type;
	int length;
	};
};

//TYPE
json_type_t json_type(const json_element_t* e){
	if(e!=json_null()) return e->type;
	return JSON_NULL;
}

//FREE
void json_free(json_element_t* e){
	json_type_t type = json_type(e);
	json_list_t c =NULL;
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
		case JSON_ARRAY: for(int i =0;i<e->length;i++){json_free(e->array[i]);} free(e->array); free(e); break;
	}
}

//BOOLEAN
json_element_t* json_boolean(bool b){
	json_element_t* e = malloc(sizeof(json_element_t*));
	e->type=JSON_BOOLEAN;
	e->boolean=b;
	return e;
	json_free(e);
}

bool json_as_boolean(const json_element_t* e){
	if(e->type!=JSON_BOOLEAN){
		printf("Error : wrong type.");
		exit(EXIT_FAILURE);
	} else return e->boolean;
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
		printf("Error : wrong type");prl;
		exit(EXIT_FAILURE);
	}else{
		return true;
	}
}

bool json_is_double(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER || e->number_type!=DOUBLE){
		printf("Error : wrong type");prl;
		exit(EXIT_FAILURE);
	}else{
		return true;
	}
}

long json_as_integer(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER){
		printf("Error : wrong type");prl;
		exit(EXIT_FAILURE);
	}else if((*e).number_type==DOUBLE){
		return (long)((*e).Double);
	}else return (*e).Integer;
}

double json_as_double(const json_element_t* e){
	if(json_type(e)!=JSON_NUMBER){
		printf("Error : wrong type");prl;
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
	strcpy(e->string,s);
	return e;
	json_free(e);
}

const char* json_as_string(const json_element_t* e){
	if(json_type(e)!=JSON_STRING){
		printf("Error : wrong type");prl;
		exit(EXIT_FAILURE);
	} else{
		return (*e).string;
	}
}	

//ARRAY
json_element_t* json_array(){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_ARRAY;
	e->array=malloc(sizeof(json_element_t*));
  e->length = 0;
	return e;
	json_free(e);
}

json_element_t* json_array_get(const json_element_t* e, size_t i){
	if(json_type(e)!=JSON_ARRAY){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	}
	if((int)i < -1 || (int)i >= e->length){
		return json_null();
	}
	else{
		return e->array[i];
	}
}

 void json_array_set(json_element_t* e, size_t i, json_element_t* se){
    if(json_type(e)!=JSON_ARRAY){
		printf("Error : wrong type");
		exit(EXIT_FAILURE);
	}
    size_t length = e->length;
    if(length == 0 && i == 0)
    {
      e->array[i] = se;
      e->length++;
    }
    else if(i > length){
        e->length= i+1;
        e->array= realloc(e->array, sizeof(json_element_t)*(i+1));

        while(length < i){
            e->array[length] = json_null();
            length++;
        }
        e->array[i] = se;
    }else{
        json_free(e->array[i]);
        e->array[i] = se;
    }
}



//OBJECT
json_element_t* json_object(){
	json_element_t* e=malloc(sizeof(json_element_t*));
	(*e).type=JSON_OBJECT;
	(*e).list = NULL;
	return e;
}


bool json_search_key(json_element_t* e,const char* key){
	bool ret = false;
	if(json_type(e)!=JSON_OBJECT){
		printf("Error : wrong type");prl;
		exit(EXIT_FAILURE);
	} else {
		json_list_t c = e->list;
		while(c !=NULL){
			if(strcmp(c->key,key) == 0){
				ret = true;
				break;
			} else c = c->next;
		}
		return ret;
	}
}

json_element_t* json_object_get(const json_element_t* e, const char* key){
	if(json_type(e)!=JSON_OBJECT){
		printf("Error : wrong type");prl;
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
		printf("Error : wrong type");prl;
		exit(EXIT_FAILURE);
	} else {
		json_list_t c = e->list;
		while(c !=NULL){
			if(strcmp(c->key,key) == 0){
				json_free((*c).value);
				(*c).value = value;
				return; 
			} else c = c->next;
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
		for(i = 0; i<(*e).length;i++){
			json_put(fd,(*e).array[i]);
			if(i != (*e).length-1){
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
    char c;
    while((c = fgetc(fd)) != EOF){
        if(c != ' ' && c != '\n' && c != '\r' && c != '\t')
            return c;
    }
    exit(EXIT_FAILURE);
}

void check_next_char(FILE* fd, char c){
    char check;
    if((check = next_char(fd)) != c){
        exit(EXIT_FAILURE);
    }
}

bool is_next_char(FILE* fd, char c, bool cons){
    char a;
    if((a = next_char(fd)) !=c || cons == false){
       ungetc(a,fd);
        return false;
  }else{
          return true;
  }
}

void check_next_word(FILE* fd, const char* w){
 char mot[strlen(w)+1];
 fread(mot, strlen(w), 1, fd);
 mot[strlen(w)] = '\0';
 if (strcmp(mot, w)) exit(EXIT_FAILURE);
}

void* myrealloc(void* ptr, size_t sz){
    ptr = realloc(ptr, sz);
    if(ptr == NULL) exit(EXIT_FAILURE);
    return ptr;
}

char* next_string(FILE* fd){
    char* string = NULL;
    int i = 0;
    char c = next_char(fd);
    while(c != '"' && c != EOF) c = next_char(fd);
    if(c == '"'){
        while((c = fgetc(fd)) != '"' && c != EOF){
            string = myrealloc(string, (i+2)*sizeof(char));
            string[i] = c;
            i++;
        }   
        string[i] = '\0';
        return string;
    }else{
        exit(EXIT_FAILURE);
    }
}

char* next_digits(FILE* fd){
    char* string = NULL;
    int i=0;
    char c = next_char(fd);
    while(('0' > c || c > '9') && c !=EOF) c = next_char(fd);
    if('0' <= c && c <= '9'){
        do{
            string = myrealloc(string, (i+2)*sizeof(char));
            string[i] = c;
            i++;
            c = fgetc(fd);
        }while('0' <= c && c <= '9' && c != EOF);
        ungetc(c, fd);
        string[i] = '\0';
        return string;
    }else{
        exit(EXIT_FAILURE);
    }   
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
      json_array_set(e,i,se);
      i++;
      c = next_char(fd);
      if (c == ']') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
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
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
        exit(EXIT_FAILURE);
      }
    }
  }
  fprintf(stderr, "Unexpected character '%c'; 'n', 't', 'f', [0-9], '\"', '[' or '{' excepted\n", c);
  exit(EXIT_FAILURE);
}


json_element_t* json_load(const char* fname){
    json_element_t* e = NULL;
    FILE* fd = NULL;
    fd = fopen(fname, "r");
    if(fd == NULL) exit(EXIT_FAILURE);
    e = json_get(fd);
    fclose(fd);
    return e;
}



