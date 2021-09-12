#include<string>
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
using namespace std;


std::string get_working_directory(){
    char path[PATH_MAX];
    return (getcwd(path, sizeof(path)) ? std::string(path) : std::string("") );
}

string sha_function(string file_name)
{
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

int untracked_fl(vector<string> &untrackedv)
{
        string localPath=get_working_directory();
        string currVersion=localPath +"\\.git\\version_No.txt";
        fstream vno(currVersion, std::ios_base::in);
        string s;  
        cout<<currVersion<<endl;
        vno >> s;
        cout << s <<endl;;
        vno.close();
        map<string, string> mp;
    for (int i = 0; i < untrackedv.size(); i++)
    {
        string filename = untrackedv[i];
        string source=localPath+"\\"+filename;
        string filesha = sha_function(source);
    

        mp[filename] = filesha;
    }
    string indexPth=localPath +"\\.git\\"+s+"\\Version_Index.txt";
    ofstream idx;
    idx.open(indexPth, ios::out);
    for (auto itr = mp.begin(); itr != mp.end(); itr++)
    {
        string line;
        line += itr->first + " " + itr->second;
        idx << line << endl;
    }
    idx.close();

    return 1;
}

vector<string> store_files(string path)
{
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

map<string, string> get_map(string path)
{
    vector<string> vec;

    vec= store_files(path);
    map<string, string> m;
    for(int i=0; i<vec.size(); i++){
        string s="";
        s=sha_function(path + vec[i]);
        m[vec[i]]=s;
    }
    return m;
}

int main()
{
   string Path=get_working_directory();
    string gitPath=Path + "\\.git";
    
    string currVersionPath= gitPath+ "\\version_No.txt";
    string baseFolder=gitPath+ "\\AllTimeFolder";
    fstream vno(currVersionPath, std::ios_base::in);
    string s;
  cout<<currVersionPath<<endl;
    vno >> s;
    cout << s <<endl;;
    vno.close();
    string curr_Version_IndexPath=gitPath + "\\" +s ;
    // string curr_Version_IndexPath =gitPath+ "\\" +s;
    cout<<curr_Version_IndexPath<<endl;
    vector<string> filelist = store_files(Path);
    cout<<Path<<endl;
    int numoffiles = filelist.size();
   /* for(int i=0; i<numoffiles; i++){
        cout<<filelist[i]<<endl;
    }*/
    map<string, string> filemap = get_map(curr_Version_IndexPath);


    vector<string> untracked;
    vector<string> modified;
    vector<string> deleted;
    for (int i = 0; i < numoffiles; i++)
    {
        string shaf = sha_function(filelist[i]);
        if (filemap.find(filelist[i]) == filemap.end())
        {
            untracked.push_back(filelist[i]);
        }
        else if (filemap[filelist[i]] != shaf)
        {
            modified.push_back(filelist[i]);
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

    
    if (untracked.size() != 0)
    {
        cout << "Untracked-Files:" << endl;
        for(int i=0; i<untracked.size(); i++){
            cout << untracked[i] << endl;
        }
    }
        
    int untracked_files = 0;

   
    if (untracked.size() != 0)
    {
        untracked_files = untracked_fl(untracked);
        if (untracked_files)
            untracked.clear();
        
    }
   
}

