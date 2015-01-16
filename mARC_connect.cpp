//---------------------------------------------------------------------------


#pragma hdrstop
                                      
#include "mARC_connect.h"

//-------------- Initialisations des statiques de classes-----
 bool    mResults::ClassReady                   = false;
 GPList* mResults::gTypes                       = NULL;
 bool    mARC_Connector::ClassReady             = false;
 GPList* mARC_Connector::MainInstances          = NULL;
 GPList* mARC_Connector::ChildInstances         = NULL;
 int     mARC_Connector::EventsCount            = 23;    //V 1.01

//________________________________________________________________
bool          mARC_Connector::OpenClass (void)
{
    if (ClassReady == true)           return true;
    MainInstances               =     new GPList;
    ChildInstances              =     new GPList;
    return true;
}
//________________________________________________________________
bool          mARC_Connector::CloseClass (void)
{
    int                 i;
    mARC_Connector      *marc;

    if (ClassReady == false) return true; //pas initialisée, donc déjà fermée
    for (i =0; i < MainInstances->Count;i++)
      {
        //on ne détruit explicitement que les éléménts de maininstance
        //les autres seront détruits par réaction en chaine
        marc = (mARC_Connector*) (*MainInstances)[i];
        delete marc;
      }
    delete MainInstances;
    delete ChildInstances;
    MainInstances = NULL;
    ChildInstances = NULL;
    ClassReady = false;
    return true;
}

//________________________________________________________________
bool          mResults::OpenClass (void)
{


    if (ClassReady == true) return true;
    gTypes = new GPList;

    GPString *tmp;

    tmp = new GPString;
    *tmp = "string";
    gTypes->Add(tmp);   //indice 0;
    tmp = new GPString;
    *tmp = "int32";
    gTypes->Add(tmp);   //indice 1;
    tmp = new GPString;
    *tmp = "uint32";
    gTypes->Add(tmp);   //indice 2;
    tmp = new GPString;
    *tmp = "int8";
    gTypes->Add(tmp);   //indice 3;
    tmp = new GPString;
    *tmp = "uint8";
    gTypes->Add(tmp);   //indice 4;
    tmp = new GPString;
    *tmp = "char";
    gTypes->Add(tmp);   //indice 5;
    tmp = new GPString;
    *tmp = "int64";
    gTypes->Add(tmp);   //indice 6;
    tmp = new GPString;
    *tmp = "uint64";
    gTypes->Add(tmp);   //indice 7;
    tmp = new GPString;
    *tmp = "string";
    gTypes->Add(tmp);   //indice 8;
    tmp = new GPString;
    *tmp = "float";
    gTypes->Add(tmp);   //indice 9;
    tmp = new GPString;
    *tmp = "double";
    gTypes->Add(tmp);   //indice 10;
    tmp = new GPString;
    *tmp = "bool";
    gTypes->Add(tmp);   //indice 11;
    tmp = new GPString;
    *tmp = "simpledate";
    gTypes->Add(tmp);   //indice 12;
    tmp = new GPString;
    *tmp = "rowid";
    gTypes->Add(tmp);   //indice 13;
    tmp = new GPString;
    *tmp = "sessionid";
    gTypes->Add(tmp);   //indice 14;

    ClassReady = true;
    return true;
}

//________________________________________________________________
bool          mResults::CloseClass (void)
{
    if (ClassReady == false) return true; //pas initialisée, donc déjà fermée
    GPString::DeleteStringList (gTypes);
    delete gTypes;
    gTypes = NULL;
    ClassReady = false;
    return true;
}


/*_______________________________________________________________
//
//     Classe  mARC_State
//_____________________________________________________________
*/
mARC_State&   mARC_State::operator = (mARC_State &C)
 {
      if (&C== this) return *this;

      LogReturn           =  C.LogReturn            ;
      LogContext          =  C.LogContext           ;
      LogResult           =  C.LogResult            ;
      LogTable            =  C.LogTable             ;
      LogKnowledge        =  C.LogKnowledge         ;
      LogServer           =  C.LogServer            ;
      DirectExecute       =  C.DirectExecute        ;
      Blocking            =  C.Blocking             ;
      ContextCall         =  C.ContextCall          ;
      ResultCall          =  C.ResultCall           ;
      TableCall           =  C.TableCall            ;
      KnowledgeCall       =  C.KnowledgeCall        ;
      ServerCall          =  C.ServerCall           ;
      LastTime            =  C.LastTime             ;
      ErrorMessage        =  C.ErrorMessage         ;
      ScriptSize          =  C.ScriptSize           ;

      return *this;
 }
//_____________________________________________________________
              mARC_State::mARC_State()
 {
      Init();
 }
//_____________________________________________________________
              mARC_State::~mARC_State()
 {

 }
//_____________________________________________________________
void          mARC_State::Init (void)
 {
     LogReturn           =      true   ;
     LogContext          =      true   ;
     LogResult           =      true   ;
     LogTable            =      true   ;
     LogKnowledge        =      true   ;
     LogServer           =      true   ;
     DirectExecute       =      false  ;
     Blocking            =      true   ;
     ContextCall         =      false  ;
     ResultCall          =      false  ;
     TableCall           =      false  ;
     KnowledgeCall       =      false  ;
     ServerCall          =      false  ;
     LastTime            =      -1.0   ;
     ErrorMessage        =      ""     ;
     ScriptSize          =      0      ;
 }

/*_______________________________________________________________
//
//     Classe  mARC_Status
//_____________________________________________________________
*/
mARC_Status&   mARC_Status::operator = (mARC_Status &C)
 {
     if (&C== this) return *this;

     Error       =   C.Error               ;
     Busy        =   C.Busy                ;
     Sending     =   C.Sending             ;
     Sent        =   C.Sent                ;
     Receiving   =   C.Receiving           ;
     Received    =   C.Received            ;

     return *this;
 }
//_____________________________________________________________
              mARC_Status::mARC_Status()
 {
    Init();
 }
//_____________________________________________________________
              mARC_Status::~mARC_Status()
 {



 }
//_____________________________________________________________
void          mARC_Status::Init (void)
 {
     Error           =    false                     ;
     Busy            =    false                     ;
     Sending         =    false                     ;
     Sent            =    false                     ;
     Receiving       =    false                     ;
     Received        =    false                     ;
 }

/*_______________________________________________________________
//
//     Classe  mResults
//_____________________________________________________________
*/

//________________________________________________________________
void  mResults::Clear(void)
{
   GPList *list;
   int i;


   GPString::ReleaseStringList(&Names);
   Types.SetCount(0);
   Sizes.SetCount(0);
   for (i=0;i<Data.Count;i++)
    {
      list = (GPList*)Data[i];
      GPString::ReleaseStringList(list);
      delete list;
    }
}

//________________________________________________________________
mResults&  mResults::operator = (mResults &C)
{
   if (&C== this) return *this;
   GPString *tmp,*src;
   GPList *list,*source;
   int i,j;
   Clear();
   for (i=0;i<C.Names.Count;i++)
    {
      tmp = GPString::Acquire();
      src = (GPString*) C.Names[i];
      *tmp = *src;
      Names.Add (tmp);
      tmp = GPString::Acquire();
      *tmp = *(GPString*) C.Types[i];
      Types.Add (tmp);
    }
   //on copie les datas
   for (i=0;i<C.Data.Count;i++)
    {
      list = new GPList;
      Data.Add(list);
      source = (GPList*)C.Data[i];
      for (j=0;j<source->Count;j++)
        {
          tmp = GPString::Acquire();
          *tmp = *(GPString*) (*source)[j];
          list->Add(tmp);
         }
    }
   lines = C.lines;
   columns = C.columns;
   return *this;
}

//________________________________________________________________
          mResults::mResults (void)
{

   if (ClassReady == false) OpenClass();
   lines=0;
   columns =0;
}

//________________________________________________________________
         mResults::~mResults (void)
{
   Clear();
}


//________________________________________________________________
bool          mResults::GetData (int line, int col, GPString* ret)
{
   if (ret == NULL)     return false;
   *ret = "";
   if (line >= lines)   return false;
   if (line < 0)        return false;
   if (col > columns)   return false;
   if (col < 0)         return false;

   GPList       *list;
   GPString     *tmp;

   list = (GPList*)  Data[line];
   if (list == NULL)    return false;
   tmp = (GPString*) (*list)[col];
   if (tmp == NULL)     return false;
   *ret = *tmp;
   return true; 
}

//________________________________________________________________
GPString*      mResults::GetData (int line, int col)
{
   if (line >= lines)   return NULL;
   if (line < 0)        return NULL;
   if (col > columns)   return NULL;
   if (col < 0)         return NULL;

   GPList       *list;
   GPString     *tmp;

   list = (GPList*)  Data[line];
   if (list == NULL)    return NULL;
   tmp = (GPString*) (*list)[col];
   return tmp;
}

//________________________________________________________________
int      mResults::GetColByName (GPString name)
{
   //renvoie l'indice de colonne pour un nom de variable
   // retourne -1 si le nom n'est pas trouvé
   if (name =="") return -1;
   name.SetCaseSensitive(false);

   int i;
   GPString     *tmp;

   for (i =0; i< Names.Count;i++)
    {
      tmp = (GPString*) Names[i];
      if (name == *tmp) return i;
    }
   return -1;
}

/*

    mArc Connector


*/


//routine d'éxécution threadée  (static)
//________________________________________________________________
void* mARC_Connector::ThExec (void *ptr)
 {
    //s'occupe de l'éxécution d'une instruction ou d'un script dans le thread du connecteur
   if (ptr == NULL) return NULL;
   try {
           mARC_Connector       *marc;

           marc = (mARC_Connector*) ptr;
           marc->Send ();
           marc->Receive ();
           marc->Status.Busy = false;
           return NULL;
       }
    catch(...)
      {
          return NULL;
      }
 }
//________________________________________________________________
void* mARC_Connector::ThTimer (void *ptr)
 {
    //s'occupe de l'éxécution d'une instruction ou d'un script dans le thread du connecteur
   if (ptr == NULL) return NULL;
   mARC_Connector       *marc;

   marc = (mARC_Connector*) ptr;
   while (!marc->Stop)
    {

    if (marc->TimerDelay <= 0)
      {
        Sleep(1000);
        continue;
      }

       marc->Event (mEvent_onMarcTimer);
       Sleep(marc->TimerDelay);
    }
   return NULL;
 }
//________________________________________________________________
void mARC_Connector::SetTimer (double time)
{
   TimerLock.Lock();
   if (time <= 0) {time = 0;}
   TimerDelay = time;

   if (TimerThread.Terminated())
    {
      TimerThread.Execute (ThTimer,this); //on lance le timer
    }
   else if (TimerThread.Suspended())
     {
       TimerThread.Resume();
     }
   TimerLock.UnLock(); 
}

//gestionnaires statiques de callback de socket
//________________________________________________________________
void        mARC_Connector::SocketError (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketError);
}
//________________________________________________________________
void        mARC_Connector::SocketConnected (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketConnected);
}
//________________________________________________________________
void        mARC_Connector::SocketReceiving (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketReceiving);
}
//________________________________________________________________
void        mARC_Connector::SocketReceived (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketReceived);

}
//________________________________________________________________
void        mARC_Connector::SocketSending (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketSending);
}
//________________________________________________________________
void        mARC_Connector::SocketSent (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketSent);
}
//________________________________________________________________
void        mARC_Connector::SocketIdle (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketIdle);
}
//________________________________________________________________
void        mARC_Connector::SocketActive (GPSocket *gpsk)
{
   mARC_Connector *marc;
   if (gpsk == NULL) return;
   if (gpsk->Link == NULL) return;
   marc = (mARC_Connector*)gpsk->Link;
   marc->Event(mEvent_onSocketActive);
}

//________________________________________________________________
bool         mARC_Connector::Event (int event)
{
   //Appelle les gestionnaires d'évènements pluggés

   int i;
   mPlug *plug;
   GPString tmp;
   bool ok;

   try {
          if (event == mEvent_onMarcError)
            {
              tmp = State.ErrorMessage;
            }

          Status.Init();
          Status.Error = false;

        switch (event)
              {
                case   mEvent_onSocketDisconnected:
                       Status.Error = true;
                       State.ErrorMessage = "socket disconnected";
                break;

                case   mEvent_onSocketError:
                       Status.Error = true;
                       State.ErrorMessage = "internal socket error";
                break;

                case   mEvent_onMarcError:
                       Status.Error = true;
                       State.ErrorMessage = tmp;
                break;

                case   mEvent_onSocketReceiving :
                       Status.Receiving = true;
                break;

                case   mEvent_onSocketReceived :
                       Status.Received = true;
                break;

                case   mEvent_onSocketSending :
                       Status.Sending = true;
                break;

                case   mEvent_onSocketSent :
                       Status.Sent = true;
                break;

                default :

                break;
              }

         ok =Plugs->Verrou.Lock(1);
         if (ok)  //on rate un évènement, mais ça évite un deadlock si ok = false
          {
             if (Plugs->Count == 0){Plugs->Verrou.UnLock();return true;}   //aucun gestionnaire d'évènements
             for (i=0; i < Plugs->Count;i++)
              {
                plug = (mPlug*) (*Plugs)[i];
                if (plug->event == event) plug->f(this,plug->object);
                Sleep(0);
              }
             Plugs->Verrou.UnLock();
          }
         return Status.Error;
        }
    catch (...)
       {
         Plugs->Verrou.UnLock();
         Status.Error = true;
         State.ErrorMessage = "internal error";
         return false;
       }
}


//________________________________________________________________
bool         mARC_Connector::UnPlug (void* object)
{
  mPlug *plug;
  int   i=0;

  if (object == NULL)return false;

  try {
       Plugs->Verrou.Lock();
       while (i < Plugs->Count)
         {
            plug = (mPlug*) (*Plugs)[i];
            if (plug->object == object)
              {
                Plugs->DeleteAt(i);
                continue;
              }
           i++;
         }
      Plugs->Verrou.UnLock();
      return true;
     }
   catch(...)
    {
      Plugs->Verrou.UnLock();
      return false;
    }
}

//________________________________________________________________
bool         mARC_Connector::Plug (void  (*f) (mARC_Connector *,void *object), void* object, int event)
{
  mPlug plug;

  if (f == NULL) return false;
  if (object == NULL)return false;
  if (event < 0) return false;             //V 1.01
  if (event >= EventsCount) return false;  //V 1.01

  try {
      plug.f = f;
      plug.object = object;
      plug.event = event;
      Plugs->Verrou.Lock();
      Plugs->Add (&plug);
      Plugs->Verrou.UnLock();
      return true;
     }
   catch(...)
    {
      Plugs->Verrou.UnLock();
      return false;
    }
}



//________________________________________________________________
          mARC_Connector::mARC_Connector (void)
{
   if (ClassReady == false)
     {
       OpenClass();
       ClassReady = true;
     }

   gpsk = NULL;
   Stop = false;
   Plugs = new GPRecord (sizeof(mPlug));
   IP = "127.0.0.1";
   Port = "1254";
   SessionId ="";
   ScriptSize = 0;
   State.DirectExecute = false; //script  des fonctions wrapper
   Parent = NULL;

   MainInstances->Verrou.Lock();
   MainInstances->Add(this);
   Name = GPIntToStr(MainInstances->Count);
   MainInstances->Verrou.UnLock();
   TimerDelay = 0;
   TimerThread.Execute (ThTimer,this);
}

//________________________________________________________________
          mARC_Connector::~mARC_Connector (void)
{
   int                  i=0;
   mARC_Connector       *marc;

  ClearScript();
  ClearResult();
  GPString::ReleaseStringList(&Params) ;

  while (ResultStack.Count > 0)
   {
     PopContext();
     ClearScript();
     ClearResult();
     GPString::ReleaseStringList(&Params) ;
    }

  //Plugs->Verrou.Lock();
  Plugs->SetCount(0);
  //Plugs->Verrou.UnLock();
  delete Plugs;
  if (gpsk != NULL) delete gpsk;

  if (Parent != NULL)
    {
      Parent->Childs.Delete(this); 
    }

   while (Childs.Count > 0)
    {
       marc = (mARC_Connector*)Childs[i];
       delete marc;
    }
     //attente d'arret des threads asynchrones
    Stop = true; //demande d'arrete des threads
    TimerThread.Terminate();
    ExecThread.Terminate();


}

//________________________________________________________________
bool          mARC_Connector::Lock (void)
{
    bool ok = false;
    int  t = 0;

    while (!ok)
     {
       ok = Verrou.TryLock();
       if (! ok)
         {
           Event (mEvent_onMarcBusy);
           Sleep (t);
           t += 10;
           if (t > 100) t = 100;    //plafonné
         }
     }
   return ok;
}
//________________________________________________________________
bool          mARC_Connector::UnLock (void)
{
   return Verrou.UnLock();
}

//________________________________________________________________
void          mARC_Connector::ClearResult (void)
{
   int          i;
   mResults     *mr;

   for (i=0;i<Result.Count;i++)
    {
      mr = (mResults*)Result[i];
      delete mr;
    }
   Result.SetCount(0);
   State.LastTime = -1.0;
}

//________________________________________________________________
void          mARC_Connector::ClearScript (void)
{
   GPString::ReleaseStringList(&Script) ;
}

//________________________________________________________________
mARC_Connector*   mARC_Connector::GetNewConnector (void)
{
  mARC_Connector*  marc;

  marc = new mARC_Connector;
  MainInstances->Verrou.Lock();
  MainInstances->Delete(marc);
  MainInstances->Verrou.UnLock();
  ChildInstances->Verrou.Lock();
  ChildInstances->Add(marc);
  ChildInstances->Verrou.UnLock();
  marc->Port = Port;
  marc->IP = IP;
  marc->Parent=this;
  Childs.Add (marc);
  marc->Name = Name + ".";
  marc->Name += GPIntToStr (Childs.Count+1);  //nom par défaut
  return marc;
}

//________________________________________________________________
bool        mARC_Connector::Connected (void)
{
   bool ok;


   if (gpsk == NULL) return false;
   if (!gpsk->Valid()) return false;
   return  gpsk->Connected();
}

//________________________________________________________________
void        mARC_Connector::Connect (void)
{
   int                  nport;
   int                  i;
   mARC_Connector       *marc;
   double               t;

    try {
           t  = TimerDelay;
           SetTimer(0);
           if (gpsk != NULL) delete gpsk;
           gpsk = new GPSocket;
           gpsk->Link= this;
           gpsk->OnConnected = mARC_Connector::SocketConnected;

           gpsk->SetBlocking(true);

           nport= GPStrToInt(Port);
           gpsk->CreateTCPClient (nport,IP);
           if (gpsk->Valid() )
           gpsk->toSend = "-1 connect ();";
           if (gpsk->Valid() ) gpsk->Send();
           if (gpsk->Valid() ) gpsk->Receive();
           int idx = 1;
           if (gpsk->Valid() )
                {
                  gpsk->toReceive.GetWord (&idx,&SessionId);
                  Event (mEvent_onSocketConnected);
                }
            else
              {
                 SessionId ="";
                 Event (mEvent_onSocketDisconnected);
              }

           Event (mEvent_onMarcConnected);   

           for (i = 0; i< Childs.Count;i++)
             {
                marc = (mARC_Connector*)Childs[i];
                marc->Port = Port;
                marc->IP = IP;
                marc->Connect();
             }

           gpsk->OnError =              mARC_Connector::SocketError;  //call back de la socket
           gpsk->OnReceived =           mARC_Connector::SocketReceived;
           gpsk->OnReceive =            mARC_Connector::SocketReceiving;
           gpsk->OnSend =               mARC_Connector::SocketSending;
           gpsk->OnSent =               mARC_Connector::SocketSent;
           gpsk->OnIdle =               mARC_Connector::SocketIdle;
           gpsk->OnActive =             mARC_Connector::SocketActive;
           SetTimer(t);

         }//end try
    catch(...)
       {

       }
}

//________________________________________________________________
bool        mARC_Connector::Valid (void)
{
  if (gpsk == NULL) return false;
  return gpsk->Valid();
}

//________________________________________________________________
void        mARC_Connector::AnalyseLine(int *idx)
{
  mResults      *mr;
  GPString       val,*tmp;
  int            i,j,size,idxtmp;
  GPList        *list;

  mr= new mResults;
  Result.Add(mr);
  gpsk->toReceive.GetWord(idx,&val);
  mr->lines = val.ToInt();
  gpsk->toReceive.GetWord(idx,&val);
  mr->columns = val.ToInt();

  //on lit les types, tailles et noms des colonnes
  for (i = 0; i < mr->columns; i++)
   {
     gpsk->toReceive.GetWord(idx,&val);      //type
     tmp = (GPString*)(*mResults::gTypes)[val.ToInt()];
     mr->Types.Add (tmp);
     gpsk->toReceive.GetWord(idx,&val);   //tailles
     size = val.ToInt();
     mr->Sizes.Add(size);
     tmp = GPString::Acquire();
     gpsk->toReceive.GetWord(idx,tmp);   //noms
     mr->Names.Add(tmp);
   }
  //on lit les données
    for (i = 0; i < mr->lines;i++)
     {
        list = new GPList;
        mr->Data.Add(list);
        for (j = 0; j < mr->columns;j++)
         {
            tmp = GPString::Acquire();
            idxtmp = *idx;
            gpsk->toReceive.GetWord (&idxtmp,tmp);
            tmp->SetCaseSensitive(false);
            if (*tmp == "NULL" || *tmp == "VOID")
              {
                 *idx = idxtmp;
              }
             else
              {
                *tmp = "";
                tmp->FromString (idx, & (gpsk->toReceive));
              }
            //gpsk->toReceive.GetWord(idx,tmp);
            //tmp->FromGPBinary();
            list->Add (tmp);
         }
     }
}

//________________________________________________________________
void        mARC_Connector::Analyse (void)
{
 //0 1 1 1 10 0 LastTime <5 4.418/> ;
 ClearResult();
 Status.Error = false;
 State.ErrorMessage = "";
 ScriptSize = 0;

 GPString       val;
 int            idx =1,len;
 int            i,j;
 mResults       *mr;

  len = gpsk->toReceive.Length();
  gpsk->toReceive.GetWord(&idx,&val); //session
  gpsk->toReceive.GetWord(&idx,&val); //erreur
  if (val =="0")
   {
     //erreur de script, on récupère le message d'erreur
     State.ErrorMessage = "code ";
     gpsk->toReceive.GetWord(&idx,&val); //code d'erreur
     State.ErrorMessage += val;
     State.ErrorMessage += " in ";
     val.FromString (&idx, & (gpsk->toReceive));
     //val.FromString (&idx, & (gpsk->toReceive));
     val.FromGPBinary();
     State.ErrorMessage += val;
     Status.Error = true;
     Event (mEvent_onMarcError);
     return;
   }
   //on analyse la première ligne de retour
   AnalyseLine(&idx);
   if (idx == 0)     return;
   if (idx == -1)    return;
   if (idx > len)    return;
   //on analyse les différentes lignes de script s'il y en a
   while (idx <= len && idx!= -1 )
    {
      //espace suivant
      while (gpsk->toReceive[idx] == ' ')
      {
        idx++ ;
        if (idx > len) return;
      } //end while gpsk
     //on doit trouver un ";"
     if (gpsk->toReceive[idx] != ';')  return;
     idx++;
     if (idx >= len) return;
     ScriptSize++;
     AnalyseLine(&idx);
     if (idx == 0) return;

   }//end while idx
}



//________________________________________________________________
bool        mARC_Connector::ExecuteCommand (GPString *cmd)
{
  //execute une commande complètement formatée, exemple
  // 1 contexts.Dup(<1 2/> ;

  GPString *line = NULL;
  int idx=1, ret,i;
  char c;

  if (cmd==NULL) return false;
  Status.Error = false;
  //ContextCall = false;
  cmd->SetCaseSensitive(false);
  if (cmd->Contains ("context"))                                {State.ContextCall = true;}
  if (cmd->Contains ("result"))                                 {State.ResultCall = true;}
  if (cmd->Contains ("table") ||cmd->Contains ("tbl") )         {State.TableCall = true;}
  if (cmd->Contains ("Knowledge") ||cmd->Contains ("knw") )     {State.KnowledgeCall = true;}
  if (cmd->Contains ("Server"))                                 {State.ServerCall = true;}
  OpenScript();

  while (true)
          {
            line = new GPString ();
            ret = cmd->GetCmdLine(&idx,line,';');
            if (ret == 7) break;
            line->Trim();
            i=1;
            //on vérifie si ce n'est pas un commentaire
            while (i<=line->Length())
             {
               c = (*line)[i];
               if (c == ' ')
                 {
                   i++;
                   continue;
                 }
               break;
             }

            if (c != '/' && *line != "") {Script.Add (line);}
            else                         {delete line;}
          }

  return  ExecuteScript();

}

//________________________________________________________________
bool        mARC_Connector::Send (void)
{
  ClearResult();

  if (gpsk == NULL)             return Event(mEvent_onSocketDisconnected);
  if (!gpsk ->Connected())      return Event(mEvent_onSocketDisconnected);
  if (!gpsk ->Valid())          return Event(mEvent_onSocketError);

  //on rajoute la demande de temps d'éxécution
  Push ("GetLastTime");
  AddFunction ();
  gpsk->toSend =  SessionId + " ";

  int i;
  GPString *tmp;
  for (i= 0; i< Script.Count;i++)
   {
     tmp = (GPString*) Script [i];
     gpsk->toSend +=  *tmp;
     gpsk->toSend +=  " ; " ;
   }
  Event (mEvent_onMarcSending);
  gpsk->Send();
  if (!gpsk ->Valid())          return Event (mEvent_onSocketError);
  else                          return Event (mEvent_onMarcSent);

}

//________________________________________________________________
bool        mARC_Connector::Receive (void)
{
  if (gpsk == NULL)             return Event(mEvent_onSocketDisconnected);
  if (!gpsk ->Connected())      return Event(mEvent_onSocketDisconnected);
  if (!gpsk ->Valid())          return Event(mEvent_onSocketError);

  Event (mEvent_onMarcReceiving);
  gpsk->Receive();
  if (!gpsk ->Valid())          {Event (mEvent_onSocketError);}
  else
     { 
                                Event (mEvent_onMarcReceived);
                                Analyse();

     }

      //on extrait le dernier resultat
  if (!Status.Error)
       {
         mResults *mr;
         GPString *tmp = NULL;
         
         mr = (mResults* )Result.Pop();
         // mr contient le LastTime de la requete
         if (mr != NULL)  tmp = mr->GetData(0,0);
         if (tmp != NULL) State.LastTime = tmp->ToDouble();
         delete mr;

           if (State.LogReturn)
           {
                                            Event(mEvent_onMarcReturn);
             if (State.ContextCall)         Event(mEvent_onMarcContext);
             if (State.ResultCall)          Event(mEvent_onMarcResult);
             if (State.TableCall)           Event(mEvent_onMarcTable);
             if (State.KnowledgeCall)       Event(mEvent_onMarcKnowledge);
             if (State.ServerCall)          Event(mEvent_onMarcServer);
           }
       }

       State.ContextCall   = false;
       State.ResultCall    = false;
       State.TableCall     = false;
       State.KnowledgeCall = false;
       State.ServerCall    = false;

      return Status.Error;

}


//________________________________________________________________
bool        mARC_Connector::ExecuteScript (void)
{
  if (State.Blocking)
   {
     Send ();
     Receive ();
     Status.Busy = false;
     return Status.Error ;
   }
  //sinon on laisse le verrouillage à la charge du thread d'éxécution
  ExecThread.Execute (ThExec,this);
  Sleep (0);
  return true;
}

//________________________________________________________________
void        mARC_Connector::OpenScript (void)
{
  if (gpsk == NULL) return;
  gpsk->toSend = "";
  ClearScript();

}

//________________________________________________________________
void        mARC_Connector::AddFunction (void)
{
   AddFunction (&Params);

}

//________________________________________________________________
void        mARC_Connector::AddFunction (GPList *params)
{
  // la liste des parametres est détruite en sortie

  if (params == NULL) return;
  if (params->Count == 0) return;
  if (gpsk == NULL)
   {
      GPString::ReleaseStringList (params);
   }


  int           i;
  GPString      *tmp;
  GPString      *sline;

  sline = GPString::Acquire();

  tmp = (GPString*)(*params)[0];    //fonction
  if (tmp == NULL) return;
  tmp->SetCaseSensitive(false);
  if (tmp->Contains ("context"))                                {State.ContextCall = true;}
  if (tmp->Contains ("result"))                                 {State.ResultCall = true;}
  if (tmp->Contains ("table") ||tmp->Contains ("tbl") )         {State.TableCall = true;}
  if (tmp->Contains ("Knowledge") ||tmp->Contains ("knw") )     {State.KnowledgeCall = true;}
  if (tmp->Contains ("Server"))                                 {State.ServerCall = true;}
  *sline += *tmp;
  *sline += " ( ";

  for (i=1;i<params->Count; i++)
    {
      tmp = (GPString*)(*params)[i];
      tmp->SetCaseSensitive(false);
      if (*tmp != "null" && *tmp != "default")
       {
          tmp->ToGPBinary();
       }
      *sline += *tmp;
      *sline += " ";
      if (i< params->Count-1)
        {
          *sline  += ", ";
        }
    }

  *sline += " )";
  GPString::ReleaseStringList (params);
  Script.Add(sline);
}


//________________________________________________
void mARC_Connector::Push (GPString s)
{
  GPString *tmp;

  tmp = GPString::Acquire();
  *tmp = s;
  Params.Add(tmp);

}

//________________________________________________
void GetAppPath (GPString *path, GPString *name, GPString *ext)
{
        //renvoi le chemin, le non, et l'extension de l'application

        int             n;
        GPString        delimiter,cfg;

        delimiter = "\\";
        cfg.SetCapacity(2048 + 1);
        n = ::GetModuleFileName(NULL, cfg.GItems, 2048);
        cfg.SetLength(n);
        GPParseFileName (&cfg,path,name,ext); 
}

//_____________________________________________
double mARC_Connector::ProcessTime(void)
{
   double ret;

   PushContext();
   Status.Error = false;
   State.DirectExecute = true;
   State.LogReturn =false; //on inhibe la maj auto de la pile de retour
   State.LogContext = false; //on inhibe la pile des contextes
   ret = GetLastTime(); 
   PopContext();
   return ret;
}

//_____________________________________________
mResults* mARC_Connector::ProcessTasks(void)
{

   mResults* ret;

   PushContext();
   State.DirectExecute = true;
   State.LogReturn =false; //on inhibe la maj auto de la pile de retour
   State.LogContext = false;
   ret = GetTasks();
   PopContext();
   return ret;
}
//________________________________________________
void mARC_Connector::PushContext (void)
{
   GPList       *newscript,*newresult;
   mARC_State   *newstate;

   newscript    =   new GPList          ;
   newresult    =   new GPList          ;
   newstate     =   new mARC_State      ;

   *newscript    =       Script;
   *newresult    =       Result;
   *newstate     =       State;

   ScriptStack.Push (newscript);
   ResultStack.Push (newresult);
   StateStack.Push  (newstate);

   Script.SetCount (0);
   Result.SetCount (0);
}
//________________________________________________
void mARC_Connector::PopContext (void)
{
   GPList       *newscript,*newresult;
   mARC_State   *newstate;

   ClearScript();
   ClearResult();

   newscript =  (GPList*)       ScriptStack.Pop();
   newresult =  (GPList*)       ResultStack.Pop();
   newstate  =  (mARC_State*)   StateStack.Pop();

   Script = *newscript;
   Result = *newresult;
   State  = *newstate;
   newscript->SetCount (0);
   newresult->SetCount (0);

   delete   newscript;
   delete   newresult;
   delete   newstate; 
}


//________________________________________________
mResults* mARC_Connector::DoIt(void)
{
   if (State.DirectExecute)     ExecuteScript ();
   if (Status.Error)            return NULL;
   if (State.Blocking)
    {
      return  (mResults*) Result[0];
    }
   return  NULL;
}
//________________________________________________
mResults* mARC_Connector::KnowledgeGetInstances (void)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("KNOWLEDGE.GET");
   Push ("INSTANCES");
   AddFunction ();
   if (State.DirectExecute)     return DoIt();
   else                         return NULL;

}

//________________________________________________
mResults* mARC_Connector::TableGetInstances (void)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("TABLE.GET");
   Push ("INSTANCES");
   AddFunction ();
   if (State.DirectExecute)     return DoIt();
   else                         return NULL;
}
//________________________________________________
double mARC_Connector::GetLastTime (void)
{

   if (State.DirectExecute)           OpenScript ();
   Push ("GetLastTime");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr==NULL) return -1.0;
       tmp = mr->GetData(0,0);
       if (tmp==NULL) return -1.0;
       return GPStrToDouble (*tmp);
    }
   else               return -1.0;
}

//________________________________________________
mResults* mARC_Connector::ContextsNew (void)
{

   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.New");
   AddFunction ();
   if (State.DirectExecute)     return DoIt();
   else                         return NULL;
}
//________________________________________________
mResults* mARC_Connector::ContextsClear (void)
{

   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.Clear");
   AddFunction ();
   if (State.DirectExecute)     return DoIt();
   else                         return NULL;
}
//________________________________________________
mResults* mARC_Connector::ContextsSetKnowledge (GPString& s)
{

   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.Set");
   Push ("Knowledge");
   Push (s);
   AddFunction ();
   if (State.DirectExecute)     return DoIt();
   else                         return NULL;
}
//________________________________________________
mResults* mARC_Connector::ContextsAddElement (GPString& s)
{

   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.AddElement");
   Push (s);
   AddFunction ();
   if (State.DirectExecute)     return DoIt();
   else                         return NULL;
}

//________________________________________________
mResults* mARC_Connector::ContextsGetStack (int max_context, int max_context_size)
{

   if (State.DirectExecute)             OpenScript ();
   Push ("Contexts.GetStack");
   Push (GPIntToStr(max_context));
   Push (GPIntToStr(max_context_size));
   AddFunction ();
   if (State.DirectExecute)             return DoIt();
   else                                 return NULL;
}

//________________________________________________
mResults* mARC_Connector::ContextsNewFromSem (bool relation ,int max_act, int max_number)
{

  if (State.DirectExecute)              OpenScript ();
   Push ("Contexts.NewFromSem");
   Push (GPBoolToStr(relation));
   Push (GPIntToStr(max_act));
   Push (GPIntToStr(max_number));
   AddFunction ();
   if (State.DirectExecute)             return DoIt();
   else                                 return NULL;

}

//________________________________________________
mResults* mARC_Connector::ContextsGetElements (int idx, int max_atoms)
{

  if (State.DirectExecute)              OpenScript ();
   Push ("Contexts.GetElements");
   Push (GPIntToStr(idx));
   Push (GPIntToStr(max_atoms));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}

//________________________________________________
int mARC_Connector::ContextsGetCount (void)
{
   if (State.DirectExecute)             OpenScript ();
   Push ("Contexts.GET");
   Push ("Count");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       l = (GPList*)mr->Data[0];
       tmp = (GPString*) (*l)[0];
       return GPStrToInt (*tmp);
    }
   else                                 return -1;

}



//________________________________________________
int mARC_Connector::ResultsGetCount (void)
{
   if (State.DirectExecute)             OpenScript ();
   Push ("Results.GET");
   Push ("Count");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       l = (GPList*)mr->Data[0];
       tmp = (GPString*) (*l)[0];
       return GPStrToInt (*tmp);
    }
   else                                 return -1;

}

//________________________________________________
mResults* mARC_Connector::ResultsGetStack (void)
{
   if (State.DirectExecute)             OpenScript ();
   Push ("Results.GET");
   Push ("Stack");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;

}

//________________________________________________
int mARC_Connector::ResultsGetFetchStart (int idx)
{
   if (State.DirectExecute)             OpenScript ();
   Push ("Results.GET");
   Push ("FetchStart");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       l = (GPList*)mr->Data[0];
       tmp = (GPString*) (*l)[0];
       return GPStrToInt (*tmp);
    }
   else                                 return -1;

}

//________________________________________________
int mARC_Connector::ResultsGetFetchSize (int idx)
{
   if (State.DirectExecute)             OpenScript ();
   Push ("Results.GET");
   Push ("GetFetchSize");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       l = (GPList*)mr->Data[0];
       tmp = (GPString*) (*l)[0];
       return GPStrToInt (*tmp);
    }
   else                                 return -1;

}
//________________________________________________
GPString* mARC_Connector::ServerGetName (void)
{
   if (State.DirectExecute)             OpenScript ();
   Push ("Server.GetName");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       mr = DoIt();
       if (mr !=NULL) return (mr->GetData (0,0));
       else           return NULL;
    }
   else                                 return NULL;

}
//________________________________________________
mResults* mARC_Connector::GetTasks (void)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Server.GetTasks");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                         return NULL;

}
//________________________________________________
mResults* mARC_Connector::GetAPI (void)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Server.GetAPI");
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                         return NULL;

}

//________________________________________________
void mARC_Connector::ContextsSortByGenerality(bool order,int idx)
{
   GPString tmp;

   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.SortByGenerality");
   Push (GPBoolToStr(order));
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       DoIt();
       return ;
    }
   else                         return ;

}

//________________________________________________
void mARC_Connector::ContextsSortByActivity(bool order,int idx)
{
   GPString tmp;

   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.SortByActivity");
   Push (GPBoolToStr(order));
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       DoIt();
       return ;
    }
   else                         return ;

}

//________________________________________________
bool mARC_Connector::ContextsGetSortOrder(int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.GetSortOrder");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       mr = DoIt();
       if (mr == NULL) return false;
       return (mr->GetData (0,0)->ToBool());
    }
   else                         return false;

}
//________________________________________________
GPString* mARC_Connector::ContextsGetSortedBy (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.GetSortedBy");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return NULL;
       tmp = mr->GetData (0,0);
       return (tmp);
    }
   else                         return NULL;

}

//________________________________________________
int mARC_Connector::ContextsGetAct (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.GetAct");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       return (mr->GetData (0,0)->ToInt());
    }
   else                         return -1;

}
//________________________________________________
int mARC_Connector::ContextsGetSize (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Contexts.GetSize");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       l = (GPList*)mr->Data[0];
       tmp = (GPString*) (*l)[0];
       return GPStrToInt (*tmp);
    }
   else                         return -1;

}

//________________________________________________
int mARC_Connector::ResultsGetSize (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.Get");
   Push ("ResultCount");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return -1;
       l = (GPList*)mr->Data[0];
       tmp = (GPString*) (*l)[0];
       return GPStrToInt (*tmp);
    }
   else                         return -1;

}


//________________________________________________
mResults* mARC_Connector::KnowledgeGetProperties (GPString *knw)
{
   GPString tmp;
   if (knw == NULL) return NULL;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Knowledge:";
   tmp += *knw;
   tmp += ".properties";
   Push (tmp);
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}
//________________________________________________
mResults* mARC_Connector::TableGetProperties (GPString *tbl)
{
   GPString tmp;
   if (tbl == NULL) return NULL;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Table:";
   tmp += *tbl;
   tmp += ".properties";
   Push (tmp);
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}

//________________________________________________
mResults* mARC_Connector::TableGetStructure (GPString *tbl)
{
   GPString tmp;
   if (tbl == NULL) return NULL;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Table:";
   tmp += *tbl;
   tmp += ".Get";
   Push (tmp);
   tmp = "Structure";
   Push (tmp);
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}

//________________________________________________
mResults* mARC_Connector::TableReadContext (GPString tbl,int dbid,int threshold)
{
   GPString tmp;
   if (tbl == NULL) return NULL;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Table:";
   tmp += tbl;
   tmp += ".ReadContext";
   Push (tmp);
   Push (GPIntToStr(dbid));
   if (threshold >=0) Push (GPIntToStr(threshold));

   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}

//________________________________________________
int mARC_Connector::TableReadBlock (GPString tbl,int dbid,GPString col,int pos,int size,GPString *retour)
{
   //retourne la prochaine position à lire, ou 0 si tout est lu ou en cas d'erreur

   GPString     tmp;
   int          idx;

   if (tbl == "") return NULL;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Table:";
   tmp += tbl;
   tmp += ".ReadBlock";
   Push (tmp);
   Push (GPIntToStr(dbid));
   Push (col);
   Push (GPIntToStr(pos));
   Push (GPIntToStr(size));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return 0;
       idx = mr->GetColByName ("Data");
       if (retour != NULL)
         {
           *retour += *mr->GetData(0,idx);
         }
       idx = mr->GetColByName ("NextPosition");
       return mr->GetData(0,idx)->ToInt();

    }
   else                                 return 0;
}

//________________________________________________
mResults* mARC_Connector::TableSelect (
                                        GPString tbl,
                                        GPString mode,
                                        GPString col,
                                        GPString op,
                                        GPString param1,
                                        GPString param2
                                        )
{
   /*
     mode : new, add, clear
     op :  <, >, <=, >=, between, =

   */


   GPString tmp;
   if (tbl == NULL) return NULL;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Table:";
   tmp += tbl;
   tmp += ".Select";
   Push (tmp);
   Push (mode);
   Push (col);
   Push (op);
   Push (param1);
   Push (param2);
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}

//________________________________________________
GPString* mARC_Connector::ResultsGetOwnerTable (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.GetOwnerTable");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return NULL;
       tmp = mr->GetData (0,0);
       return (tmp);
    }
   else                         return NULL;

}

//________________________________________________
GPString* mARC_Connector::ResultsGetSortedBy (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.GetSortedBy");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return NULL;
       tmp = mr->GetData (0,0);
       return (tmp);
    }
   else                         return NULL;

}

//________________________________________________
GPString* mARC_Connector::ResultsGetSortOrder (int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.GetSortOrder");
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPString *tmp;
       mr = DoIt();
       if (mr == NULL) return NULL;
       tmp = mr->GetData (0,0);
       return (tmp);
    }
   else                         return NULL;

}

//________________________________________________
mResults* mARC_Connector::ResultsSortBy (int idx,GPString colname,bool sortorder)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.SortBy");
   Push (GPIntToStr(idx));
   Push (colname);
   Push (GPBoolToStr(sortorder));
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       mr = DoIt();
       return mr;
    }
   else                         return NULL;

}

//________________________________________________
void mARC_Connector::ResultsSetFormat (GPString format,int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.SetFormat");
   Push (format);
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       DoIt();
    }

}

//________________________________________________
void mARC_Connector::ResultsSetFetchStart (int start,int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.SetFetchStart");
   Push (GPIntToStr(start));
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       DoIt();
    }

}

//________________________________________________
void mARC_Connector::ResultsSetFetchSize (int size,int idx)
{
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.SetFetchSize");
   Push (GPIntToStr(size));
   Push (GPIntToStr(idx));
   AddFunction ();
   if (State.DirectExecute)
    {
       DoIt();
    }

}

//________________________________________________
mResults* mARC_Connector::ResultsFetch (int idx, int lines, int start)
{
   //si lines <= 0 valeurs par défaut, fetchsize et fetchsttart
   if (State.DirectExecute)     OpenScript ();
   Push ("Results.Fetch");
   Push (GPIntToStr(idx));
   if (lines > 0)
    {
       Push (GPIntToStr(lines));
       if (start > 0)
         {
           Push (GPIntToStr(start));
         }
    }

   AddFunction ();
   if (State.DirectExecute)
    {
       return DoIt();
    }
   return NULL;
}

//________________________________________________
mResults* mARC_Connector::ServerGetProperties (void)
{
   GPString tmp;
   if (State.DirectExecute)             OpenScript ();
   tmp = "Properties";
   Push (tmp);
   AddFunction ();
   if (State.DirectExecute)
    {
       mResults* mr;
       GPList *l;
       GPString *tmp;
       mr = DoIt();
       return mr;
    }
   else                                 return NULL;
}


#pragma package(smart_init)
