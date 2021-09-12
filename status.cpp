#include<iostream>
#include<bits/stdc++.h>
#include<unistd.h>
#include<stdio.h>
#include<limits.h>
#include<dirent.h>
#include<sys/types.h>
#include<fstream>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
using namespace std;


std::string get_working_directory(){
    char path[PATH_MAX];
    return (getcwd(path, sizeof(path)) ? std::string(path) : std::string("") );
}

string sha_function(string file_name){
    string shaF;
    string cmd = "Certutil -hashfile " + file_name + " SHA1 | findstr -v file " +"> temp.txt";
    system(cmd.c_str());
    ifstream sha_file("temp.txt");
    getline(sha_file, shaF);
    shaF.erase(remove(shaF.begin(), shaF.end(), ' '), shaF.end());
    sha_file >> shaF;
    sha_file.close();
    remove("temp.txt");
    return shaF;
}
/*int listdir(const char *path) {
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path);
    if (dp == NULL) {
        perror("opendir: Path does not exist or could not be read.");
        return -1;
    }

    while ((entry = readdir(dp)))
        string ent="";
        string ent=(string)entry->d_name;
        string s=sha_function(ent);
        cout<<s<<endl;


    closedir(dp);
    return 0;
}*/
vector<string> store_files(string path){
    vector<string> stFile;
    struct dirent *entry;
    DIR *dir =opendir(path.c_str());
    if(dir == NULL){
        cout<<"Error in opening directorty";
        exit(EXIT_FAILURE);
    }
    while((entry = readdir(dir)) != NULL){
        // cout<< entry->d_name<<endl;
        string f=entry->d_name;
        if(f == "." || f == ".." || f == ".git" || f == ".vscode" || f == ".cph" || f == "Version_Index.txt"){
            continue;
        }else{
            stFile.push_back(f);
        }
    }
    closedir(dir);
    return stFile;
}

map<string, string> get_map(string path){
   vector<string> vec;

    vec= store_files(path);
    map<string, string> m;
    for(int i=0; i<vec.size(); i++){
        string s="";
        s=sha_function(path + "\\"+vec[i]);
        m[vec[i]]=s;
        cout<<vec[i]<<s<<endl;
    }
    return m;
}
int main(){
    string Path=get_working_directory();
    string gitPath=Path + "\\.git";
    
    string currVersionPath= gitPath+ "\\version_No.txt";
    fstream vno(currVersionPath, std::ios_base::in);
    string s;
    // vno.open("version_No.txt", ios::in);    
  cout<<currVersionPath<<endl;
    vno >> s;
    cout << s <<endl;;
    vno.close();
    // string curr_Version_IndexPath=gitPath + "\\" +s +"\\Version_Index.txt";
    string curr_Version_IndexPath =gitPath+ "\\AllTimeFolder";
    cout<<curr_Version_IndexPath<<endl;
    vector<string> filelist = store_files(Path);
    cout<<Path<<endl;
    int numoffiles = filelist.size();
   /* for(int i=0; i<numoffiles; i++){
        cout<<filelist[i]<<endl;
    }*/
    map<string, string> filemap = get_map(curr_Version_IndexPath);
    vector<string> untracked; //new
    vector<string> modified;
    vector<string> deleted;
    // string path="C:\\c-programming\\";
    for (int i = 0; i < numoffiles; i++)
    {
        string shaf = sha_function(Path +"\\"+ filelist[i]);
        // cout<<shaf<<endl;
        if (filemap.find(filelist[i]) == filemap.end())
        {
            // cout << "filelist[i]" << filelist[i] << endl;
            untracked.push_back(filelist[i]);
        }
        else
        {
            // string s=sha_function("C:\\c-programming\\MyGit.cpp");
            // cout<<filelist[i]<<endl;
                            // cout<<s<<endl;
        // cout<<filemap[filelist[i]]<<endl;
            if (filemap[filelist[i]] != shaf)
            {
                modified.push_back(filelist[i]);
            }
        }
    }
    sort(filelist.begin(), filelist.end());
    for (auto f : filemap)
    {
        if (find(filelist.begin(), filelist.end(), f.first) == filelist.end())
        {
            deleted.push_back(f.first);
        }
    }
    cout << "no. of modified files " << modified.size() << endl;
    cout << "no. of untracked files " << untracked.size() << endl;
    cout << "no. of deleted files " << deleted.size() << endl;
    // string p=sha_function(Path+"\\MyGit.cpp");
    // cout<<p<<endl;
    return 0;
}