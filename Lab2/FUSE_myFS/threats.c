
int data;

void Reader(void) {
	 while(1){
		sem_wait(&sem_nreaders);
		nr_readers = nr_readers + 1;
		if (nr_readers == 1)
		 	sem_wait(&sem_read_write);
		sem_post(&sem_nreaders);
		/* read data */
		printf("%d\n", data);
		sem_wait(&sem_nreaders);
		nr_readers = nr_readers - 1;
		if (nr_readers == 0)
		 	sem_post(&sem_read_write);
		sem_post(&sem_nreaders);
	 }
}

void Writer(void) {
 	while(1){
		 sem_wait(&sem_read_write);
		 /* modify the resource */
		 data = data + 2;
		 sem_post(&sem_read_write);
	}
}

int main(int argc, char argv[]) {
	Reader();
	Writer();
}