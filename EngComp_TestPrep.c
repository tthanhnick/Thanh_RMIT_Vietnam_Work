#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int c;

// bài 1: xuất nội dung file ra màn hình
int showContent(char *name) {
	FILE *fi = fopen("test.txt", "w+");
	if (fi == NULL)
		return 0;
	else {
		char str[100], ch, str1[100];
		printf("Enter a string: ");
		scanf("%[^\n]s", str1);
		fprintf(fi, "%s", str1);

		do {
			// lấy 1 dòng từ file
			fscanf(fi, "%[^\n]s", str);
			if (fscanf(fi, "%c", &ch) != EOF) {
				// xuất ra màn hình sau đó xuống dòng
				printf("%s\n", str);
			} else
				break;
		} while (1);

		fclose(fi);
		return 1;
	}
}

int countWord(char *str) {
	int i, words = 0;
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] == ' ' || str[i] == 9) &&
			(str[i - 1] != ' ' && str[i - 1] != 9)) {
			words++;
		}
	if (str[i - 1] != ' ' && str[i - 1] != 9)
		words++;
	return words;
}
// bài 4
int fileCount(char *name) {
	FILE *fi = fopen("test.txt", "r");
	if (fi == NULL)
		return 0;
	else {
		char str[100], ch;
		int numberLine = 0;
		int numberWord = 0;
		do {
			fscanf(fi, "%[^\n]s", str);
			if (fscanf(fi, "%c", &ch) != EOF) {
				numberLine++;
				numberWord += countWord(str);
			} else
				break;
		} while (1);

		printf(
			"FILENAME: %s , LINE: %d , WORDS: %d\n",
			name,
			numberLine,
			numberWord);
		fclose(fi);
		return 1;
	}
}

// bài 2,3: copy thông tin file1 vào file2 (bài 3 có thêm số dòng)
int fileCopy(char *name1, char *name2, int hasLine) {
	FILE *fi = fopen("test.txt", "r");
	if (fi == NULL)
		return 0; // mở file để copy fail => dừng
	else {
		// mở file2 , nếu tồn tại thì viết đè
		FILE *fo = fopen("test2.txt", "w+");
		if (fo == NULL)
			fo = fopen(
				"test2.txt", "a"); // không tồn tại thì tạo cía mới để viết

		char str[100], ch;
		int numberLine = 1;
		do {
			// lấy từng dòng
			fscanf(fi, "%[^\n]s", str);
			// kiểm tra xem có phải kết thúc chưa (EOF viết tắt của end of file,
			// nếu fscanf không đọc được tới cuối file sẽ trả về EOF)
			if (fscanf(fi, "%c", &ch) != EOF) {
				// kiểm tra xem có cần thêm số dòng trước mỗi câu không (bài 3)
				if (hasLine == 1) {
					fprintf(fo, "Line %d:", numberLine++);
				}
				// in dòng vừa lấy từ tập tin 1 vào tập tin 2
				fprintf(fo, "%s\n", str);
			} else
				break;
		} while (1);

		fclose(fi);
		fclose(fo);
		return 1;
	}
}

// bài 8: kiểm tra 2 file có y chang nhhau không, nếu có thì xuất equal
// không thì xuất dòng khác nhau
int fileCompare(char *name1, char *name2) {
	FILE *fi = fopen("test.txt", "r"), *fo = fopen("test2.txt", "r");
	if (fi == NULL || fo == NULL)
		return 0; // nếu 1 trong 2 file không tồn tại thì trả về fail
	else {
		char str[100], str2[100], ch;
		int numberLine = 1;
		do {
			// lấy từng dòng file 1 ra so với dòng ở file2
			fscanf(fi, "%[^\n]s", str);
			if (fscanf(fi, "%c", &ch) != EOF) {
				fscanf(fo, "%[^\n]s", str2);
				if (fscanf(fo, "%c", &ch) != EOF) {
					// có khác nhau
					if (strcmp(str, str2) != 0) {
						printf(
							"Differ at line %d\nFile %s: %c%s%c\nFile %s: "
							"%c%s%c\n",
							numberLine,
							name1,
							34,
							str,
							34,
							name2,
							34,
							str2,
							34);
						return 1;
					}
				} else {
					// file2 end trước
					printf(
						"Differ at line %d\nText in file %s is end\n",
						numberLine,
						name2);
					return 1;
				}
			} else {
				fscanf(fo, "%[^\n]s", str2);
				if (fscanf(fo, "%c", &ch) != EOF) {
					// file1 end trước
					printf(
						"Differ at line %d\nText in file %s is end\n",
						numberLine,
						name1);
					return 1;
				} else {
					// 2 file đểu end chung => như nhau
					printf("Equal\n");
					return 1;
				}
			}
			numberLine++;
		} while (1);

		fclose(fi);
		fclose(fo);
		return 1;
	}
}
// bài 9
int fileAppend(char *name1, char *name2) {
	FILE *fi = fopen("test.txt", "a"), *fo = fopen("test2.txt", "r");
	if (fi == NULL || fo == NULL)
		return 0; // nếu 1 trong 2 file không tồn tại => fail
	else {
		char str[100], ch;
		do {
			// lấy dòng file2
			fscanf(fo, "%[^\n]s", str);
			if (fscanf(fo, "%c", &ch) != EOF) {
				// nối vào file1
				fprintf(fi, "%s\n", str);
			} else
				break;
		} while (1);

		fclose(fi);
		fclose(fo);
		return 1;
	}
}

// các câu lệnh tương ứng
char cmd[6][20] = {"./copy",
				   "./copywithlinenum",
				   "./count",
				   "./compare",
				   "./append",
				   "./show"};
int nCmd = 6;

// Xử lý câu lệnh (bài 5 trở đi)
int processCmd(char str[]) {
	char splitStr[4][20] = {"", "", "", ""};

	// cắt lệnh
	int i = 0, j = 0, run = 0;
	while (str[j] != '\0') {
		if (str[j] != ' ') {
			splitStr[run][i++] = str[j];
		} else if (str[j - 1] != ' ') {
			splitStr[run][i] = '\0';
			run++;
			i = 0;
		}
		j++;
	}
	// VD:$ ./copy temp1.txt temp2.txt
	// cắt ra
	// splitStr[0] = "$"
	// splitStr[1] = "./copy"
	// splitStr[2] = "temp1.txt"
	// splitStr[3] = "temp2.txt"

	// Kiểm tra xem bắt đầu phải là dấu $ không
	if (strcmp(splitStr[0], "$") == 0) {
		// kiểm tra xem câu lênh thực thi ứng với lệnh nào
		for (i = 0; i < nCmd; i++)
			if (strcmp(splitStr[1], cmd[i]) == 0) {
				switch (i) {
				case 0:
				case 1:
					// dùng cho câu lệnh copy (bài 2 và 3)
					return fileCopy(splitStr[2], splitStr[3], i);
				case 2:
					// dùng cho lệnh show thông tin (bài 4)
					return fileCount(splitStr[2]);
				case 3:
					// dùng cho lệnh so sánh 2 tập tin txt (bài 8)
					return fileCompare(splitStr[2], splitStr[3]);
				case 4:
					// dùng cho lệnh nối tập tin 2 vào tập tin 1 (bài 9)
					return fileAppend(splitStr[2], splitStr[3]);
				case 5:
					return showContent(splitStr[2]);
				}
				break;
			}
		if (i == nCmd)
			return 0;
	}
	// này là dùng để thoát thôi
	else if (strcmp(splitStr[0], "exit") == 0)
		exit(0);
	// không phải thì cmd bị lỗi , trả về 0
	return 0;
}

int main() {
	while (1) {
		char str[100];
		printf("\nCMD > ");
		scanf("%[^\n]s", str);
		getchar();

		if (processCmd(str) == 1)
			printf("DONE");
		else
			printf("FAIL");

		str[0] = '\0';
	}
}
