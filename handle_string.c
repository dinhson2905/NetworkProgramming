char * get_data(char command[]) {
	int i = 0, j;
	while (command[i] != ' ') {
		i++;
	}
	i++;

	char * data = malloc(LENGTH_MSG);
	for (j = 0; i+j < strlen(command); j++) {
		data[j] = command[i+j];
	}
	data[j] = '\0';
	return data;
}

void convert_room_detail(char *data) {
	int i = 0, j, k = 0;
	char element[10];
	while(data[i]) {
		memset(element, 0, strlen(element));
		j = 0;
		while(data[i] != '-') {
			element[j++] = data[i++]; 
		}
		room_arr[k].id = atoi(element);
		memset(element, 0, strlen(element));
		j = 0;
		while(data[i] != '#') {
			element[j++] = data[i++]; 
		}
		room_arr[k].client_num = atoi(element);
		k++; // room index
		i++; // string index
	}
}
char * get_friend_name(char str[]) {
	char *temp= malloc(30*sizeof(char));
	int  i = 0, j = 0;
	while(str[i] != ' ') {
		temp[j++] = str[i++];
	}
	temp[j] = '\0';
	return temp;
}
void convert_client_detail(char *data) {
	int i = 0, j, k = 0;
	char element[10];
	while(data[i]) {
		memset(element, 0, strlen(element));
		j = 0;
		while(data[i] != '#') {
			element[j++] = data[i++]; 
		}
		element[j] = '\0'; 
		i++;
		strcpy(client_arr[k++].name, element);   
	}
	running_client = k;
}

char * string_multiline(char *str) {
	char *temp = malloc(LENGTH_MSG*sizeof(char));
	int  i = 0, j = 0;
	while(i < strlen(str)) {
		if (str[i] == '#') {
			temp[j++] = '\n';
			i++;
		}
		else temp[j++] = str[i++];
	}
	temp[j] = '\0';
	return g_locale_to_utf8(temp, -1, 0, 0, 0);
	//return temp;
}