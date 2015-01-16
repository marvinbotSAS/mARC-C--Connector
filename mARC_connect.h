
/*
* Copyright (C) 2015 Marvinbot S.A.S
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

//---------------------------------------------------------------------------
/*
 mArc Connector V 1.0
 creation date  16/12/2011
 author NRMX

 current : 1.01
 Ajout de la static Event qui maintient le nombre d'�v�nements g�r�s
 Modif de Plug sur le test de coh�rence du num�ro d'�v�nement



 Connector object to a mArc Server
 it uses the gpilib library

 a connector is Event Driven, and fully asynchronous
 each connector owns a socket and a session to a mArcServer
 it is multhreading safe

 It's purpose is to execute and receive results, and Events from the server
 and to dispatch the results among plugged object

 the object executing a command, is not necessarly the object receiving the result
 each event can be sent to several plugged objects

 a connector can have childs automatically connected to the same server, but each having a different session
 a child connector can be explicitely destroyed
 when a parent connector is destroyed, all his children's connector are destroyed
 a connector can be in blocking mode or not
 if blocking == true, a method calling the connector's ExecuteScript method will wait until the result is received

 it is possible to temporarly inhibit an event through the state of the followin properties
 bool LogReturn;
 bool LogContext;
 bool LogResult;
 bool LogTable;
 bool LogKnowledge;
 bool LogServer;

 ____________________________________________________________
 Pluging un object example :
 let's have a class "whatever" pluged on the event  "onMarcReturn"

 hpp :
 class whatever
 {
   static  void onMarcReturn (mARC_Connector *marc,void *object); //the event handler of that class

 };

 cpp :
 void whatever::onMarcReturn (mARC_Connector *marc,void *object)
 {
    if (marc == NULL) return;
    if (object == NULL) return;

    whatever *ptr;
    ptr = (whatever*) object;
    ptr->AnyMethod (marc);

    // do what you want using the marc
    // typically exploiting marc->Result in this case
 }

mARC_Connector   connector;
whatever         example1, example2;

connector.Plug (whatever::onMarcReturn,example1,mEvent_onMarcReturn);
connector.Plug (whatever::onMarcReturn,example2,mEvent_onMarcReturn);

//each object example1, and example2 will receive the callback of the connector when a new result is received

 

*/

#ifndef mARC_connectH
#define mARC_connectH


#include "gpilib.h"

//d�clarations forward
class mARC_State;
class mARC_Status;
class mPlug;
class mResults;
class mARC_Connector;

#define mEvent_onSocketConnected     (int) (0)
#define mEvent_onSocketDisconnected  (int) (1)
#define mEvent_onSocketError         (int) (2)
#define mEvent_onSocketReceiving     (int) (3)
#define mEvent_onSocketReceived      (int) (4)
#define mEvent_onSocketSending       (int) (5)
#define mEvent_onSocketSent          (int) (6)
#define mEvent_onSocketIdle          (int) (7)
#define mEvent_onSocketActive        (int) (8)
#define mEvent_onMarcReturn          (int) (9)
#define mEvent_onMarcContext         (int) (10)
#define mEvent_onMarcTable           (int) (11)
#define mEvent_onMarcKnowledge       (int) (12)
#define mEvent_onMarcServer          (int) (13)
#define mEvent_onMarcBusy            (int) (14)
#define mEvent_onMarcTimer           (int) (15)
#define mEvent_onMarcSending         (int) (16)
#define mEvent_onMarcSent            (int) (17)
#define mEvent_onMarcReceiving       (int) (18)
#define mEvent_onMarcReceived        (int) (19)
#define mEvent_onMarcResult          (int) (20)
#define mEvent_onMarcError           (int) (21)
#define mEvent_onMarcConnected       (int) (22)
#define mEvent_onServerEvent         (int) (23)




//---------------------------------------------------------------------------
class mARC_State
{
    public :
    bool                LogReturn;    //activation / inhibition �v�nement mEvent_onMarcReturn
    bool                LogContext;   //activation / inhibition �v�nement mEvent_onMarcContext
    bool                LogResult;    //activation / inhibition �v�nement mEvent_onMarcResult
    bool                LogTable;     //activation / inhibition �v�nement mEvent_onMarcTable
    bool                LogKnowledge; //activation / inhibition �v�nement mEvent_onMarcKnowledge
    bool                LogServer;    //activation / inhibition �v�nement mEvent_onMarcServer
    bool                DirectExecute;  //true, execution directe des fonctions, false, scriptage des fonctions wrapper
    bool                Blocking;       //appel bloquant ou non
    bool                ContextCall;   //ev�nement d�tect�  mEvent_onMarcContext
    bool                ResultCall;    //ev�nement d�tect�  mEvent_onMarcResult
    bool                TableCall;     //ev�nement d�tect�  mEvent_onMarcTable
    bool                KnowledgeCall; //ev�nement d�tect�  mEvent_onMarcKnowledge
    bool                ServerCall;    //ev�nement d�tect�  mEvent_onMarcServer
    int                 ScriptSize;
    double              LastTime;      //dur�e du dernier script/commandet �x�cut�
    GPString            ErrorMessage;  //dernier message d'erreur
    mARC_State&   operator = (mARC_State &C);       //operateur d'affectation
                  mARC_State();
                 ~mARC_State();
    void          Init (void);
};



class mARC_Status
{
    public :
    bool                Error;
    bool                Busy;
    bool                Sending;
    bool                Sent;
    bool                Receiving;
    bool                Received;
    mARC_Status&  operator = (mARC_Status &C);       //operateur d'affectation
                  mARC_Status();
                 ~mARC_Status();
    void          Init  (void);
};

class mPlug
{
  public:
  void  (*f) (mARC_Connector *,void *object);
  void  *object;
  Int32   event;
};


//---------------------------------------------------------------------------
class mResults
{


    static  bool  ClassReady;
    static  bool  OpenClass ();
    static  bool  CloseClass ();
 public:
    static GPList *gTypes;
    int lines;
    int columns;

    GPList Names ;
    GPList Types ;
    GPList Sizes ;
    GPList Data  ;    //liste de liste de string

                mResults();
                ~mResults();
    void        Clear (void);
    mResults&   operator = (mResults &C);       //operateur d'affectation

    bool        GetData (int line,int col, GPString *ret);
    GPString*   GetData (int line,int col);
    int         GetColByName (GPString name);

};



/*
un mARC_Connector est asynchrone, et �v�nementiel
il contient sa propre connection serveur.
plusieurs connecteurs peuvent �tr chain�s, vers un m�me serveur via
les propri�t�s Parent et Child
reconnecter l'un d'entre eux, les reconnecte tous


�v�nements g�r�s :

OnContext       quand une commande de type contextXXX a �t� �mise
OnResult        quand une commande de type resultXXX a �t� �mise
onTable         quand une commande de type table insert, delete ou update a �t� �mise
OnError         quand une fonction retourne une erreur
onReturn        quand un nouveau R�sultat est obtenu
onServerEvent   quand un �v�nement serveur a �t� d�tect�
onSocketError   quand une erreur de communication a eu lieu

enregistrement d'un gestionnaire


*/



//---------------------------------------------------------------------------
class mARC_Connector
{
  public:
   static GPList *MainInstances;    //liste des connecteurs primordiaux
   static GPList *ChildInstances;   //liste des connecteurs enfants
   static int    EventsCount;       //nombre d'�v�nement d�finis

  private :
   static  bool  ClassReady;
   static  bool  OpenClass ();
   static  bool  CloseClass ();

   GPThread           ExecThread ;
   static void*       ThExec (void*);      //thread d'�x�cution si Blocking = true
   GPThread           TimerThread ;
   static void*       ThTimer (void*);     //thread du timer int�gr�

   void               TimerSuspend() {TimerThread.Suspend();};
   //callbacks vers la GPSocket int�gr�e
   static void        mARC_Connector::SocketConnected          (GPSocket *gpsk);
   static void        mARC_Connector::SocketError              (GPSocket *gpsk);
   static void        mARC_Connector::SocketReceiving          (GPSocket *gpsk);
   static void        mARC_Connector::SocketReceived           (GPSocket *gpsk);
   static void        mARC_Connector::SocketSending            (GPSocket *gpsk);
   static void        mARC_Connector::SocketSent               (GPSocket *gpsk);
   static void        mARC_Connector::SocketIdle               (GPSocket *gpsk);
   static void        mARC_Connector::SocketActive             (GPSocket *gpsk); //la socket fait qq chose (le contraire de idle)


  protected :


     GPRecord           *Plugs;         //les gestionnaies d'�v�nements enregistr�s
     GPString           ScriptSession;
     GPList             Params;        //Piles des commandes et params de script
     int                ScriptSize;
     GPList             ResultStack;   //pour sauvegarde/restauration des contextes de connecteur
     GPList             StateStack;    //pour sauvegarde/restauration des contextes de connecteur
     GPList             ScriptStack;   //pour sauvegarde/restauration des contextes de connecteur

     double             TimerDelay;
     GPLock             TimerLock;

     //m�thodes
    bool        isValid(void);
    void        Analyse(void);          //analyse la chaine de retour, et alimente le tableau Result
    void        AnalyseLine(int *idx);
    mResults*   DoIt (void);            //�x�cution imm�diate, usage interne only
    void        ClearResult(void);
    void        ClearScript(void);
    bool        Receive (void);
    bool        Send (void);
    void        AddFunction (GPList *params);


 public:
    GPLock             Verrou;
    GPSocket           *gpsk;         // La socket cliente vers le serveur mARC

    GPString            IP;             //adresse IP defaut 127.0.0.1
    GPString            Port;           // Port d�faut 1254
    GPString            SessionId;
    GPString            Name;

    mARC_Connector::mARC_Connector (void);
    mARC_Connector::~mARC_Connector (void);

    mARC_Connector     *Parent;        //le connecteur Parent s'il existe, NULL sinon
    GPList             Childs;         //une liste de connecteurs enfants

    //gestion �v�nementielle
    bool        Event (int eventnum);   //g�n�ration d'�v�nements
    bool Plug   (void  (*f) (mARC_Connector *,void *object), void* object, int event);
    bool UnPlug (void* object);
    //activation / inactivation des �v�nements
    mARC_State          State;
    // Satus de connexions et d'�x�cution
    mARC_Status         Status;
    GPList              Result;                 //une list de mResults
    GPList              Script;                 //des lignes de script
    //obtention d'un connecteur li�
    mARC_Connector*  GetNewConnector (void);    //si le parent est d�truit, celui ci aussi
    void Connect(void);                         //connexion � un serveur
    void PushContext (void);                    //sauvegarde le contexte du connecteur
    void PopContext  (void);                    //restaure le contexte du connecteur

    bool ExecuteCommand (GPString *cmd);
    bool ExecuteScript (void);
    void OpenScript (void);
    void AddFunction (void);
    void Push (GPString s);       //empile une commande ou un param sur la pile de script
    void SetTimer (double time);
    bool Lock(void);
    bool UnLock(void);

    bool Connected();
    bool Valid();
    bool Stop;                     //arret des process asynchrones
    double ProcessTime(void);      //renvoi la dur�e du dernier script �x�cut�
    mResults* ProcessTasks(void);  //renvoi les t�ches serveurs en cours
 
    // mARC API wraper
    mResults* KnowledgeGetInstances      (void);
    mResults* TableGetInstances          (void);
    mResults* GetTasks                   (void);
    mResults* GetAPI                     (void);
    double    GetLastTime                (void);
    GPString* ServerGetName              (void);
    mResults* ContextsGetStack           (int max_context, int max_context_size);
    int       ContextsGetCount           (void);
    bool      ContextsGetSortOrder       (int idx);
    GPString* ContextsGetSortedBy        (int idx);
    void      ContextsSortByGenerality   (bool order,int idx);
    void      ContextsSortByActivity     (bool order,int idx);
    int       ContextsGetAct             (int idx);
    int       ContextsGetSize            (int idx);
    mResults* ContextsGetElements        (int idx, int max_atoms);
    mResults* ContextsNew                (void);
    mResults* ContextsClear              (void);
    mResults* ContextsSetKnowledge       (GPString &know);
    mResults* ContextsAddElement         (GPString &know);
    mResults* ContextsNewFromSem         (bool relation ,int max_act, int max_number);
    mResults* KnowledgeGetProperties     (GPString *knw);
    mResults* TableGetProperties         (GPString *tbl);
    mResults* TableGetStructure          (GPString *tbl);
    mResults* TableReadContext           (GPString tbl,int dbid,int threshold);
    int       TableReadBlock             (GPString tbl,
                                          int dbid,
                                          GPString col,
                                          int pos,
                                          int size,
                                          GPString *retour
                                          );

    mResults* TableSelect                (GPString tbl,
                                           GPString mode,
                                           GPString col,
                                           GPString op,
                                           GPString param1,
                                           GPString param2
                                           );

    mResults* ServerGetProperties        (void);
    int       ResultsGetCount            (void);
    int       ResultsGetSize             (int idx);
    int       ResultsGetFetchStart       (int idx);
    int       ResultsGetFetchSize        (int idx);
    GPString* ResultsGetOwnerTable       (int idx);
    GPString* ResultsGetSortedBy         (int idx);
    GPString* ResultsGetSortOrder        (int idx);
    mResults* ResultsSortBy              (int idx,GPString colname,bool sortorder);
    mResults* ResultsGetStack            (void);
    void      ResultsSetFormat           (GPString format, int idx  );
    void      ResultsSetFetchStart       (int start, int idx  );
    void      ResultsSetFetchSize        (int size, int idx  );
    mResults* ResultsFetch               (int idx, int lines, int start);
};




    //utilities
void      GetAppPath                 (GPString *path, GPString *name, GPString *ext);

#endif
