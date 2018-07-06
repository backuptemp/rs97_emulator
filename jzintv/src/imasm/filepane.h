#ifndef FILEPANE_H_INCLUDED
#define FILEPANE_H_INCLUDED

#include "includes.h"

enum
{
    FP_PROJECT = 0,
    FP_FOLDER,
    FP_SOURCE,
    FP_INCLUDE,
    FP_OTHER,
    FP_SIZE
};

typedef struct _TAG_ITEMPARAM
{
    string sName;
    string sPath;
} ItemParam;

class FilePane
{
public:
    FilePane(HWND hWnd);
    ~FilePane();
    int LoadDefaultPane();
    int LoadProject(const char *szPath);
    int LoadProject(string &sPath);
    int AddSourceFile(string &sName, string &sPath);
    int AddSourceFile(const char *szName, const char *szPath);
    int AddIncludeFile(string &sName, string &sPath);
    int AddIncludeFile(const char *szName, const char *szPath);
    int AddOtherFile(string &sName, string &sPath);
    int AddOtherFile(const char *szName, const char *szPath);
    int Resize(RECT &rc);
    int Resize(int x, int y, int cx, int cy);
    int GetCurSelPath(char *szBuff, int iMax);
    bool IsOpen() {return m_bHaveProject;}
    int NewProject(const char *szName, const char *szPath);
    int NewProject(string &sName, string &sPath);
    int RemoveSelected();

private:
    int CreateTreeView();
    void CreateImageList();
    void DeleteNodes(HTREEITEM hRoot);
    HTREEITEM AddItem(HTREEITEM hParent, string &sItem, int iType, ItemParam *pIP = NULL, bool bSort = true);
    HTREEITEM AddItem(HTREEITEM hParent, const char *szItem, int iType, ItemParam *pIP = NULL, bool bSort = true);
    int UpdateItem(HTREEITEM hItem, const char *szItem, ItemParam *pIP = NULL);
    int WriteProject();

    HIMAGELIST m_hImgList;
    HTREEITEM m_hTreeRoot;
    HTREEITEM m_hTreeSource;
    HTREEITEM m_hTreeInclude;
    HTREEITEM m_hTreeOther;

    HWND m_hParent;
    HWND m_hTree;

    //string m_sProj;
    //LList<string> m_fileList;
    int m_anImages[FP_SIZE];
    bool m_bHaveProject;
};

#endif
