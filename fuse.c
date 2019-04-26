#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#define KEY 17

static const char *dirpath = "/home/bayulaxana/shift4";
char *chyper = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

void encode(char *str) {
    if (!strcmp(str, ".")) return;
    if (!strcmp(str, "..")) return;
    
    int len = strlen(str), i, j;
    for(i=0; i<len; i++) 
    {
        char tmp = str[i];
        for(j=0; j<94; j++) {
            str[i] = (tmp == chyper[j]? chyper[(j+KEY)%94] : str[i]);
        }
    }
}

void decode(char *str) {
    if (!strcmp(str, ".")) return;
    if (!strcmp(str, "..")) return;
    
    int len = strlen(str), i, j;
    for(i=0; i<len; i++)
    {
        char tmp = str[i];
        for(j=0; j<94; j++) {
            str[i] = (tmp == chyper[j]? chyper[(j+94-KEY)%94] : str[i]);
        }
    }
}

// static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
// {
// 	int fd;

// 	fd = open(path, fi->flags, mode);
// 	if (fd == -1)
// 		return -errno;

// 	fi->fh = fd;
// 	return 0;
// }

// static int xmp_write(const char *path, const char *buf, size_t size,
// 		     off_t offset, struct fuse_file_info *fi)
// {
// 	int res;

// 	(void) path;
// 	res = pwrite(fi->fh, buf, size, offset);
// 	if (res == -1)
// 		res = -errno;

// 	return res;
// }

static int xmp_mkdir(const char *path, mode_t mode)
{
	char new_name[1000], fpath[1000];
	sprintf(new_name, "%s", path);
	encode(new_name);
	sprintf(fpath, "%s%s",dirpath,new_name);
	int res;

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
	char fpath[1000], new_name[1000];
    
    sprintf(new_name, "%s", path);
    encode(new_name);

	sprintf(fpath,"%s%s",dirpath,new_name);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000], new_name[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
        // sprintf(fpath, "%s%s",dirpath,path);
        sprintf(new_name, "%s", path);
        encode(new_name);
        sprintf(fpath, "%s%s", dirpath, new_name);
    }
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		char tmp[10000];
        struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
        strcpy(tmp, de->d_name);
        decode(tmp);
		res = (filler(buf, tmp, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
    char fpath[1000], new_name[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
        // sprintf(fpath, "%s%s",dirpath,path);
        sprintf(new_name, "%s", path);
        encode(new_name);
        sprintf(fpath, "%s%s", dirpath, new_name);
    }
	int res = 0;
    int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	// .create		= xmp_create,
	// .write		= xmp_write,
	.mkdir		= xmp_mkdir,
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}