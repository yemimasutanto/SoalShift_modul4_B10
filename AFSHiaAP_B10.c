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
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#define KEY 17
#define TRUE 1
#define FALSE 0

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

static int xmp_flush(const char *path, struct fuse_file_info *fi)
{
	int res;

	(void) path;
	/* This is called from every close on an open file, so call the
	   close on the underlying filesystem.	But since flush may be
	   called multiple times for an open file, this must not really
	   close the file.  This is important if used on a network
	   filesystem like NFS which flush the data/metadata on close() */
	res = close(dup(fi->fh));
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
	char new_name[1221], fpath[1221];
	sprintf(new_name, "%s", path);
	encode(new_name);
	sprintf(fpath, "%s%s",dirpath,new_name);

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	char new_name[1221], fpath[1221];
	sprintf(new_name, "%s", path);
	encode(new_name);
	sprintf(fpath, "%s%s",dirpath,new_name);

	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
	char new_name[1221], fpath[1221];
	sprintf(new_name, "%s", path);
	encode(new_name);
	sprintf(fpath, "%s%s",dirpath,new_name);


	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char new_name[1221], fpath[1221];
	sprintf(new_name, "%s", path);
	encode(new_name);
	sprintf(fpath, "%s%s",dirpath,new_name);

	res = lchown(fpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to, unsigned int flags)
{
	
	char n_from[1221], n_to[1221];
	char source_name[1221], dest_name[1221];
	int res;

	sprintf(n_from, "%s", from);
	sprintf(n_to, "%s", to);

	encode(n_from);
	encode(n_to);

	sprintf(source_name, "%s%s", dirpath,n_from);
	sprintf(dest_name, "%s%s", dirpath, n_to);

	/* When we have renameat2() in libc, then we can implement flags */
	if (flags)
		return -EINVAL;

	res = rename(source_name, dest_name);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int fd;
	char new_name[1221], fpath[1221];
	sprintf(new_name, "%s", path);
	encode(new_name);
	sprintf(fpath, "%s%s",dirpath,new_name);

	fd = open(fpath, fi->flags, mode);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000], name[1000];
	sprintf(name,"%s",path);
    encode(name);
	sprintf(fpath, "%s%s",dirpath,name);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

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
		char tmp[11511]; // Used to store the decoded string
		char tmp2[11511]; // Used to 
		struct stat st;
		struct stat tmstat;
		/* --------------------------------- */
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		strcpy(tmp, de->d_name);
		sprintf(tmp2, "%s/%s", fpath,tmp);
		decode(tmp);

		/* Nomor 3 --> Starting to check for username chipset or ic_controller */

		stat(tmp2, &tmstat);
		struct passwd *username = getpwuid(tmstat.st_uid);
		struct group *grup = getgrgid(tmstat.st_gid);

		printf("%s %s\n",username->pw_name, grup->gr_name);

		int isUsernameChipset = strcmp(username->pw_name, "chipset");
		int isUsernameIc = strcmp(username->pw_name, "ic_controller");
		int checkGroup = strcmp(grup->gr_name, "rusak");

		isUsernameChipset = (isUsernameChipset==0 ? TRUE : FALSE);
		isUsernameIc = (isUsernameIc==0 ? TRUE : FALSE);
		checkGroup = (checkGroup==0 ? TRUE : FALSE);

		FILE *f_pointer;
		char to_miris[1221], time_str[1221], buffer[1221];

		if ((isUsernameChipset==TRUE || isUsernameIc==TRUE) && checkGroup==TRUE && !(tmstat.st_mode & 0444))
		{
			printf("%d %d %d\n",isUsernameChipset, isUsernameIc, checkGroup);
			char filemiris[1000];

			strcpy(to_miris, dirpath);
			strcat(filemiris, "/filemiris.txt");
			encode(filemiris);
			
			strcat(to_miris, filemiris);

			f_pointer = fopen(to_miris, "a+"); // a+ untuk append //
			time_t now_time = time(NULL);
			
			strftime(time_str, 50, "%H:%M:%S %D-%m-%Y", localtime(&tmstat.st_atime));
			fprintf(f_pointer, "%s\t%d:%d\t%s\t%s\n", time_str, username->pw_uid, grup->gr_gid, path, tmp);
			fclose(f_pointer);
			remove(tmp2);
			
		}
		else {
			res = (filler(buf, tmp, &st, 0));
			if(res!=0) break;
		}
		
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
	.flush		= xmp_flush,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.rename		= xmp_rename,
	.create		= xmp_create,
	.write		= xmp_write,
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