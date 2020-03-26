/*
 * $Id$
 */

/*
 * REDBFCDX RDD (ver.2)
 *
 * Copyright 1999 Bruno Cantero <bruno@issnet.net>
 * Copyright 2003 Przemyslaw Czerpak <druzus@acn.waw.pl>
 * Copyright 2006-2008 Miguel Angel Marchuet <miguelangel@marchuet.net>
 * www - http://www.xharbour.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#ifndef HB_RDDCDX_H_
#define HB_RDDCDX_H_

#include "hbrdddbf.h"

HB_EXTERN_BEGIN

/* CDX constants and defaults */
#define CDX_INDEXEXT                              ".cdx"
#define CDX_MAXKEY                                  240
#define CDX_MAXEXP                                  255
#define CDX_MAXTAGNAMELEN                            10
#define CDX_PAGELEN                                 512
#define CDX_HEADERLEN                              1024
#define CDX_HEADEREXPLEN          (CDX_HEADERLEN - 512)
#define CDX_HEADERPAGES   ((CDX_HEADERLEN+CDX_PAGELEN-1)/CDX_PAGELEN)
#define CDX_INT_FREESPACE              (CDX_PAGELEN-12) /* 500 */
#define CDX_EXT_FREESPACE              (CDX_PAGELEN-24) /* 488 */
#define CDX_DUMMYNODE                       0xFFFFFFFFL

/* #define CDX_LOCKOFFSET                      0x7FFFFFFEL */
/* #define CDX_LOCKSIZE                                 1L */
#define CDX_STACKSIZE                                64
#define CDX_PAGECACHESIZE                             8
#define CDX_NODE_BRANCH                               0
#define CDX_NODE_ROOT                                 1
#define CDX_NODE_LEAF                                 2
#define CDX_NODE_UNUSED                            0xFF
#define CDX_IGNORE_REC_NUM                         0x0L
#define CDX_MAX_REC_NUM                     0xFFFFFFFFL
#define CDX_BALANCE_LEAFPAGES                         3
#define CDX_BALANCE_INTPAGES                          3

#define CDX_CURKEY_UNDEF                        (1<< 0)
#define CDX_CURKEY_REC                          (1<< 1)
#define CDX_CURKEY_VAL                          (1<< 2)
#define CDX_CURKEY_INPAGE                       (1<< 3)
#define CDX_CURKEY_INSTACK                      (1<< 4)
#define CDX_CURKEY_NOTEXIST                     (1<< 5)
#define CDX_CURKEY_RAWCNT                       (1<< 6)
#define CDX_CURKEY_RAWPOS                       (1<< 7)
#define CDX_CURKEY_LOGCNT                       (1<< 8)
#define CDX_CURKEY_LOGPOS                       (1<< 9)

#define TOP_RECORD                                    1
#define BTTM_RECORD                                   2
#define PREV_RECORD                                   3
#define NEXT_RECORD                                   4
#define PRVU_RECORD                                   6
#define NXTU_RECORD                                   5

#define NODE_NEWLASTKEY                               1
#define NODE_SPLIT                                    2
#define NODE_JOIN                                     4
#define NODE_BALANCE                                  8
#define NODE_EAT                                     16

#define CURKEY_RAWCNT(pTag)   (((pTag)->curKeyState & CDX_CURKEY_RAWCNT) != 0)
#define CURKEY_LOGCNT(pTag)   (((pTag)->curKeyState & CDX_CURKEY_LOGCNT) != 0)
#define CURKEY_SETLOGCNT(pTag, lKeyCount) do { (pTag)->curKeyState |= CDX_CURKEY_LOGCNT; \
                                               (pTag)->logKeyCount = (lKeyCount); } while(0)

#define CURKEY_RAWPOS(pTag)   ( ((pTag)->curKeyState & CDX_CURKEY_RAWPOS) != 0 && \
                                 (pTag)->rawKeyRec == (pTag)->CurKey->rec )
#define CURKEY_SETRAWPOS(pTag) do { (pTag)->curKeyState |= CDX_CURKEY_RAWPOS; \
                                    (pTag)->rawKeyRec = (pTag)->CurKey->rec; } while(0)

#define CURKEY_LOGPOS(pTag)   ( ((pTag)->curKeyState & CDX_CURKEY_LOGPOS) != 0 && \
                                 (pTag)->logKeyRec == (pTag)->pIndex->pArea->dbfarea.ulRecNo )
#define CURKEY_SETLOGPOS(pTag) do { (pTag)->curKeyState |= CDX_CURKEY_LOGPOS; \
                                    (pTag)->logKeyRec = (pTag)->pIndex->pArea->dbfarea.ulRecNo; } while(0)

/*
#define CURKEY_UNDEF(pTag)    (((pTag)->curKeyState & CDX_CURKEY_UNDEF) != 0)
#define CURKEY_NOTEXIST(pTag) (((pTag)->curKeyState & CDX_CURKEY_NOTEXIST) != 0)
#define CURKEY_ISSET(pTag)    (((pTag)->curKeyState & (CDX_CURKEY_NOTEXIST | CDX_CURKEY_UNDEF)) == 0)
#define CURKEY_REC(pTag)      ((((pTag)->curKeyState & CDX_CURKEY_REC) != 0) ? (pTag)->curKey->rec : 0)
#define CURKEY_VAL(pTag)      ((((pTag)->curKeyState & CDX_CURKEY_VAL) != 0) ? (pTag)->curKey->val : NULL)
#define CURKEY_REFRESH(pTag)
*/

#define HB_CDXMAXKEY( x )     ((USHORT) ((x) > CDX_MAXKEY ? CDX_MAXKEY : (x)))
#define HB_CDXBITMASK( x )    ((LONG) ((1L<<(x))-1))

/* #define FAST_GOCOLD( A )      ((A)->dbfarea.fRecordChanged || (A)->fCdxAppend ? (SELF_GOCOLD((AREAP)(A))) : HB_SUCCESS) */
#define FAST_GOCOLD( A )      SELF_GOCOLD(A)


#define CDX_TYPE_UNIQUE        0x01    /* unique index */
#define CDX_TYPE_TEMPORARY     0x02    /* temporary index */
#define CDX_TYPE_CUSTOM        0x04    /* custom index */
#define CDX_TYPE_FORFILTER     0x08    /* for expression present */
#define CDX_TYPE_BITVECTOR     0x10    /* SoftC? */
#define CDX_TYPE_COMPACT       0x20    /* FoxPro */
#define CDX_TYPE_COMPOUND      0x40    /* FoxPro */
#define CDX_TYPE_STRUCTURE     0x80    /* FoxPro */

#define CDX_CMP_EXACT          0x00    /* exact comparision */
#define CDX_CMP_PREFIX         0x01    /* prefix comparision */
#define CDX_CMP_DATE           0x02    /* date comparision */

/*
 SIx3 order temperature flags:
   switch ( indexOpt & ( CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM ) )
      case CDX_TYPE_TEMPORARY:
         PARTIAL_RYO
      case CDX_TYPE_CUSTOM:
         PARTIAL_RYO | CHGONLY_RYO
      case CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM:
         PARTIAL_RYO | NOUPDATE_RYO
         if index key begin with:
            'SXCHAR(' or 'SXNUM(' or 'SXDATE(' or 'SXLOG('
         then
            | TEMPLATE_RYO

   sx_chill()  if ( ! NOUPDATE_RYO ) then set ( CHGONLY_RYO | PARTIAL_RYO )
                  if ( indexOpt & ( CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM ) !=
                        CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM )
                  {
                     indexOpt &= ~CDX_TYPE_CUSTOM;
                     indexOpt |= CDX_TYPE_TEMPORARY
                  }

   sx_warm()   if ( ! NOUPDATE_RYO ) then clear CHGONLY_RYO
                  if ( indexOpt & ( CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM ) !=
                        CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM )
                  {
                     indexOpt |= CDX_TYPE_CUSTOM;
                     indexOpt &= ~CDX_TYPE_TEMPORARY
                  }

   sx_freeze() set NOUPDATE_RYO
                  indexOpt |= CDX_TYPE_TEMPORARY | CDX_TYPE_CUSTOM;
*/

/* CDX index node strucutres */
/* Compact Index Header Record */
typedef struct _CDXTAGHEADER
{
   BYTE     rootPtr  [ 4 ];   /* offset of the root node */
   BYTE     freePtr  [ 4 ];   /* offset of list of free pages or -1 */
   BYTE     reserved1[ 4 ];   /* Version number ??? */
   BYTE     keySize  [ 2 ];   /* key length */
   BYTE     indexOpt;         /* index options see CDX_TYPE_* */
   BYTE     indexSig;         /* index signature */
   BYTE     reserved2[ 478 ];
   BYTE     codepage[ 5 ];    /* VFP codepage */
   BYTE     ignoreCase;       /* 1 = ignore case, key converted to upper */
   BYTE     reserved3[ 2 ];
   BYTE     ascendFlg[ 2 ];   /* 0 = ascending  1 = descending */
   BYTE     forExpPos[ 2 ];   /* offset of filter expression */
   BYTE     forExpLen[ 2 ];   /* length of filter expression */
   BYTE     keyExpPos[ 2 ];   /* offset of key expression */
   BYTE     keyExpLen[ 2 ];   /* length of key expression */
   BYTE     keyExpPool[ CDX_HEADEREXPLEN ];
} CDXTAGHEADER;
typedef CDXTAGHEADER * LPCDXTAGHEADER;

/* Compact Index Interior Node Record */
typedef struct _CDXINTNODE
{
   BYTE     attr    [ 2 ];    /* node type see CDX_NODE_* */
   BYTE     nKeys   [ 2 ];    /* number of keys */
   BYTE     leftPtr [ 4 ];    /* offset of left node or -1 */
   BYTE     rightPtr[ 4 ];    /* offset of right node or -1 */
   BYTE     keyPool [ CDX_INT_FREESPACE ];
} CDXINTNODE;
typedef CDXINTNODE * LPCDXINTNODE;
typedef CDXINTNODE CDXNODE;
typedef CDXNODE * LPCDXNODE;

/* Compact Index Exterior Node Record */
typedef struct _CDXEXTNODE
{
   BYTE     attr    [ 2 ];    /* node type see CDX_NODE_* */
   BYTE     nKeys   [ 2 ];    /* number of keys */
   BYTE     leftPtr [ 4 ];    /* offset of left node or -1 */
   BYTE     rightPtr[ 4 ];    /* offset of right node or -1 */
   BYTE     freeSpc [ 2 ];    /* free space available in a page */
   BYTE     recMask [ 4 ];    /* record number mask */
   BYTE     dupMask;          /* duplicate bytes count mask */
   BYTE     trlMask;          /* trailing bytes count mask */
   BYTE     recBits;          /* number of bits for record number */
   BYTE     dupBits;          /* number of bits for duplicate count */
   BYTE     trlBits;          /* number of bits for trailing count */
   BYTE     keyBytes;         /* total number of bytes for recnn/dup/trail info */
   BYTE     keyPool [ CDX_EXT_FREESPACE ];      /* rec/dup/trl */
} CDXEXTNODE;
typedef CDXEXTNODE * LPCDXEXTNODE;



/* CDX internal memory structures */

struct _CDXAREA;  /* forward declaration */
struct _CDXINDEX; /* forward declaration */
struct _CDXTAG;   /* forward declaration */

typedef struct _CDXKEY
{
   BYTE *   val;
   USHORT   len;
   USHORT   mode;
   ULONG    rec;
} CDXKEY;
typedef CDXKEY * LPCDXKEY;

typedef struct _CDXPAGE
{
   ULONG    Page;
   ULONG    Left;
   ULONG    Right;

   BYTE     PageType;
   int      iKeys;
   int      iCurKey;

   BOOL     fChanged;
   BYTE     bUsed;

   ULONG    RNMask;
   BYTE     ReqByte;
   BYTE     RNBits;
   BYTE     DCBits;
   BYTE     TCBits;
   BYTE     DCMask;
   BYTE     TCMask;
   BOOL     fBufChanged;
   union
   {
      CDXEXTNODE extNode;
      CDXINTNODE intNode;
   } node;
   BYTE     bufKeyVal[ CDX_MAXKEY ];      /* buffer for leaf key val or added branch key */
   SHORT    bufKeyNum;                    /* do not change these vars' order             */
   SHORT    bufKeyPos;                    /* they have to be just after the node         */
   SHORT    bufKeyLen;                    /* and maybe temporary overwriten when adding  */
   SHORT    iFree;                        /* new key to interior node record.            */
   BYTE *   pKeyBuf;                      /* pointer to uncompressed leaf page key pool  */
   /* SHORT    iKeyInBuf; */

   struct _CDXPAGE * Owner;
   struct _CDXPAGE * Child;
   struct _CDXTAG  * TagParent;
   struct _CDXPAGE * pPoolPrev;
   struct _CDXPAGE * pPoolNext;
} CDXPAGE;
typedef CDXPAGE * LPCDXPAGE;

typedef struct _CDXSTACK
{
   LPCDXPAGE Page;
   int       iKey;
} CDXSTACK;
typedef CDXSTACK * LPCDXSTACK;

typedef struct _CDXLIST
{
   ULONG    ulAddr;
   BOOL     fStat;
   struct _CDXLIST * pNext;
} CDXLIST;
typedef CDXLIST * LPCDXLIST;

typedef struct _CDXTAG
{
   char *   szName;           /* Name of tag */
   char *   KeyExpr;          /* a tag key expression as text */
   char *   ForExpr;          /* a tag for expression as text */
   PHB_ITEM pKeyItem;         /* item with a macro pcode for a tag key expression */
   PHB_ITEM pForItem;         /* item with a macro pcode for a tag for expression */
   USHORT   uiType;           /* a type of key expression value */
   USHORT   uiLen;            /* length of the key expression value */
   USHORT   nField;           /* Field number for simple (one field) key expersion */
   BYTE     bTrail;           /* trailing character for shorter key value */
   BYTE     OptFlags;         /* index options flag */
   BOOL     AscendKey;        /* ascending/descending order flag */
   BOOL     UniqueKey;        /* unique order flag */
   BOOL     Temporary;        /* temporary order flag */
   BOOL     Custom;           /* custom order flag */
   BOOL     Template;         /* user keyadata in ordKeyAdd()/ordKeyDel() accepted */
   BOOL     MultiKey;         /* repeated key values in custom indexes accepted */
   BOOL     Partial;          /* order is updated only partially - missing keys possible */
   BOOL     ChgOnly;          /* only existing key modifications are updated, no new key added */
   BOOL     UsrAscend;        /* user settable ascending/descending order flag */
   BOOL     UsrUnique;        /* user settable unique order flag */
   BOOL     IgnoreCase;       /* ignore case (upper keys) */

   BOOL     TagChanged;
   BOOL     TagBOF;
   BOOL     TagEOF;

   BOOL     fRePos;
   int      curKeyState;      /* see: CDX_CURKEY_* */
   ULONG    rawKeyCount;
   ULONG    rawKeyPos;
   ULONG    rawKeyRec;
   ULONG    logKeyCount;
   ULONG    logKeyPos;
   ULONG    logKeyRec;

   ULONG    TagBlock;         /* a page offset where a tag header is stored */
   ULONG    RootBlock;        /* a page offset with the root of keys tree */
   USHORT   MaxKeys;          /* maximum number of keys in Interior node */

   struct _CDXINDEX * pIndex; /* a parent index info */
   struct _CDXTAG   * pNext;  /* pointer to next tag in index */

   /* CDXSTACK  PageStack[ CDX_STACKSIZE ]; */  /* stack with page path to current key */
   LPCDXPAGE RootPage;        /* pointer to root of keys tree in memory */
   LPCDXKEY  CurKey;          /* current value of key expression */
   LPCDXKEY  HotKey;          /* value of hot key expression */
   BOOL      HotFor;          /* index FOR condition for HotKey */

   PHB_ITEM  topScope;        /* Top scope HB_ITEM */
   LPCDXKEY  topScopeKey;     /* Top scope index key */
   PHB_ITEM  bottomScope;     /* Bottom scope HB_ITEM */
   LPCDXKEY  bottomScopeKey;  /* Bottom index key */

   LPCDXPAGE pagePool;        /* page buffer in memory */
} CDXTAG;
typedef CDXTAG * LPCDXTAG;

typedef struct _CDXINDEX
{
   char *      szFileName;    /* Name of index file */
   char *      szRealName;    /* Real name of index file */
   PHB_FILE    pFile;         /* Index file handle */
   struct _CDXAREA  * pArea;  /* Parent WorkArea */
   struct _CDXINDEX * pNext;  /* The next index in the list */
   LPCDXTAG    pCompound;     /* Compound tag */
   LPCDXTAG    TagList;       /* List of tags in index file */
   BOOL        fShared;       /* Shared file */
   BOOL        fReadonly;     /* Read only file */
   BOOL        fDelete;       /* delete on close flag */
   USHORT      uiRemote;      /* Flag to determine if order is remote 0 : native 1 : local 2 : remote */    
   ULONG       nextAvail;     /* offset to next free page in the end of index file */
   ULONG       freePage;      /* offset to next free page inside index file */
   LPCDXLIST   freeLst;       /* list of free pages in index file */
   int         lockWrite;     /* number of write lock set */
   int         lockRead;      /* number of read lock set */
   HB_FOFFSET  ulLockPos;     /* readlock position for CL53 lock scheme */
#ifdef HB_CDX_DBGCODE
   BOOL        RdLck;
   BOOL        WrLck;
#endif
   BOOL        fChanged;      /* changes written to index, need upadte ulVersion */
   ULONG       ulVersion;     /* network version/update flag */
   BOOL        fFlush;        /* changes written to index, need upadte ulVersion */
} CDXINDEX;
typedef CDXINDEX * LPCDXINDEX;

/* for index creation */
typedef struct
{
   HB_FOFFSET  nOffset;    /* offset in temporary file */
   ULONG       ulKeys;     /* number of keys in page */
   ULONG       ulKeyBuf;   /* number of keys in memory buffer */
   ULONG       ulCurKey;   /* current key in memory buffer */
   BYTE *      pKeyPool;   /* memory buffer */
} CDXSWAPPAGE;
typedef CDXSWAPPAGE * LPCDXSWAPPAGE;

typedef struct
{
   LPCDXTAG pTag;             /* current Tag */
   PHB_FILE pTempFile;        /* pointer to temporary file */
   char *   szTempFileName;   /* temporary file name */
   int      keyLen;           /* key length */
   BYTE     bTrl;             /* filler char for shorter keys */
   BOOL     fUnique;          /* TRUE if index is unique */
   BOOL     fReindex;         /* TRUE if reindexing is in process */
   ULONG    ulMaxRec;         /* the highest record number */
   ULONG    ulTotKeys;        /* total number of keys indexed */
   ULONG    ulKeys;           /* keys in curently created page */
   ULONG    ulPages;          /* number of pages */
   ULONG    ulCurPage;        /* current page */
   ULONG    ulPgKeys;         /* maximum number of key in page memory buffer */
   ULONG    ulMaxKey;         /* maximum number of keys in single page */
   BYTE *   pKeyPool;         /* memory buffer for current page then for pages */
   LPCDXSWAPPAGE pSwapPage;   /* list of pages */
   LPCDXPAGE NodeList[ CDX_STACKSIZE ];   /* Stack of pages */
   ULONG    ulFirst;
   ULONG *  pSortedPages;
   BYTE     pLastKey[ CDX_MAXKEY ]; /* last key val */
   ULONG    ulLastRec;
   BYTE *   pRecBuff;
#ifndef HB_CDX_PACKTRAIL
   int      iLastTrl;         /* last key trailing spaces */
#endif
} CDXSORTINFO;
typedef CDXSORTINFO * LPCDXSORTINFO;



/*
 *  DBF WORKAREA
 *  ------------
 *  The Workarea Structure of DBFCDX RDD
 *
 */

typedef struct _CDXAREA
{
   DBFAREA dbfarea;

   /*
   *  CDX's additions to the workarea structure
   *
   *  Warning: The above section MUST match DBFAREA exactly! Any
   *  additions to the structure MUST be added below, as in this
   *  example.
   */

   LPCDXSORTINFO  pSort;         /* Index build structure */
   LPCDXINDEX     lpIndexes;     /* Pointer to indexes array  */
   BYTE *         bCdxSortTab;   /* Table with sorted characters */
   BOOL           fCdxAppend;    /* Appended record changed */
   BOOL           fSortCDP;      /* Use CDP functions for sorting */
   USHORT         uiTag;         /* current tag focus */

} CDXAREA;

typedef CDXAREA * LPCDXAREA;

#ifndef CDXAREAP
#define CDXAREAP LPCDXAREA
#endif

/* m Bitmap, b Size, r RecNo */
#define BM_SetBit(m,b,r) ((r)<=(b))?((m)[((r)-1)>>5] = (m)[((r)-1)>>5] | (1<<(((r)-1)%32))):0
#define BM_ClrBit(m,b,r) ((r)<=(b))?((m)[((r)-1)>>5] = (m)[((r)-1)>>5] & ~(1<<(((r)-1)%32))):0
#define BM_GetBit(m,b,r) (((r)<=(b))?(((m)[((r)-1)>>5] & (1<<(((r)-1)%32)))):0)

typedef struct _BM_FILTER_ {
    PHB_ITEM itmCobExpr;       /* Block representation of the FILTER expression */
    PHB_ITEM abFilterText;     /* String representation of FILTER expression */
    BOOL     fFilter;          /* flag to indicate that filter is active */
    BOOL     fOptimized;       /* Is (should be) filter optimized */
    ULONG*   rmap;
    ULONG    Size;
} BM_FILTER;

typedef BM_FILTER * LPBM_FILTER;

#undef  SUPERTABLE
#define SUPERTABLE                         ( &cdxSuper )

HB_EXTERN_END

#endif /* HB_RDDREC_H_ */
