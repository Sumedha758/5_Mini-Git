#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <dirent.h>
#include <cstdlib>

using namespace std;
void copyFile(const char *fileNameFrom, const char *fileNameTo)
{

    char buff[BUFSIZ];
    FILE *in, *out;
    size_t n;

    in = fopen(fileNameFrom, "rb");
    out = fopen(fileNameTo, "wb");
    while ((n = fread(buff, 1, BUFSIZ, in)) != 0)
    {
        fwrite(buff, 1, n, out);
    }
}

int dir(std::string path)
{

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {

        while ((ent = readdir(dir)) != NULL)
        { /* print all the files and directories within directory */
            if (ent->d_name != std::string("."))
            { //REMOVE PWD
                if (ent->d_name != std::string(".."))
                { //REMOVE PARENT DIR

                    std::cout << path << "\\" << ent->d_name << std::endl;
                }
            }
        }

        std::cout << std::endl;

        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("");
        return EXIT_FAILURE;
    }

    return 0;
}

int copyAllFiles(std::string sorc, std::string dest)
{

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(sorc.c_str())) != NULL)
    {

        while ((ent = readdir(dir)) != NULL)
        { /* print all the files and directories within directory */
            if (ent->d_name != std::string("."))
            { //REMOVE PWD
                if (ent->d_name != std::string(".."))
                { //REMOVE PARENT DIR

                    std::string copy_sorc = sorc + "\\" + ent->d_name;
                    std::string copy_dest = dest + "\\" + ent->d_name;

                    std::cout << "cp " << copy_sorc << " -> " << copy_dest << std::endl;

                    copyFile(copy_sorc.c_str(), copy_dest.c_str());
                }
            }
        }

        std::cout << std::endl;

        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("");
        return EXIT_FAILURE;
    }

    return 0;
}

bool init(void)
{
    if (mkdir(".git") == -1)
    {
        cerr << "Error1 : " << strerror(errno) << endl;
        return false;
    }

    if (chdir(".git") == -1)
    {
        cerr << "Error2 :" << strerror(errno) << endl;
        return false;
    }

    if (mkdir("AllTimeFolder") == -1)
    {
        cerr << "Error1 :" << strerror(errno) << endl;
        return false;
    }

    
    copyAllFiles("C:\\mini-git", "C:\\mini-git\\.git\\AllTimeFolder"); //COPY FOLDER'S CONTENT


    fstream vno;
    vno.open("version_No.txt", ios::out);
    if (!vno)
    {
        return false;
    }
    else
    {
        string number = "0";
        vno << number << endl;
    }
    vno.close();

    fstream GitLog;
    GitLog.open("log.txt", ios::out);
    if (!GitLog)
    {
        return false;
    }

    if (mkdir("0") == -1)
    {
        cerr << "Error1 :" << strerror(errno) << endl;
        return false;
    }
    if (chdir("0") == -1)
    {
        cerr << "Error2 : " << strerror(errno) << endl;
        return false;
    }

    fstream VersionIndex;
    VersionIndex.open("Version_Index.txt", ios::out);
    if (!VersionIndex)
    {
        return false;
    }
    return false;
}
void explore(char *dir_name)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    dir = opendir(dir_name);
    if (!dir)
    {
        cout << "Directory was not found\n"
             << endl;
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            string path = string(dir_name) + "/" + string(entry->d_name);
            cout << "Entry =" << path << endl;
            stat(path.c_str(), &info);
            if (S_ISDIR(info.st_mode))
            {
                explore((char *)path.c_str());
            }
        }
    }
    closedir(dir);
}
void add(void)
{
    cout << "files added" << endl;
    ;
}
void status(void)
{
    cout << "status"
         << "\n";
}
void commit(void)
{
    cout << "files commited"
         << "\n";
}
void rollBack(void)
{
    cout << "RollBacked to verion v2"
         << "\n";
}
int main(int argc, char **argv)
{
    string s;
    s = argv[1];
    bool NewRepo;
    //  explore((char*)".");
    if (s == "init")
    {
        NewRepo = init();
    }
    else if (s == "add")
    {
        add();
    }
    else if (s == "status")
    {
        if (NewRepo == false)
        {
            cout << "Not a git repository." << endl;
        }
        else
        {
            status();
        }
    }
    else if (s == "commit")
    {
        commit();
    }
    else if (s == "rollback")
    {
        rollBack();
    }
    else
    {
        cout << "" << s << "is not a git commmand." << endl;
        cout << "init: Create an empty Git repository or reinitialize an existing" << endl;
        cout << "add : Add file contents to the index" << endl;
        cout << "status: Show the working tree status" << endl;
        cout << "commit: Record changes to the repository" << endl;
        cout << "log: Show the changes logged" << endl;
    }
    return 0;
}
