#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <string.h>
#include "SOUNDEX.c"
#include "btree.h"
#include <time.h>
enum{
  COL_WORD = 0,
  COL_NUM
};

int ok=1;
struct timespec ts, start, end;

BTA *evdata;
BTA *sound;

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *combobox;
GtkWidget *find_entry;
GtkTreeView *word_treeview;
GtkTextView *mean_textview;
GtkWidget *find_button;
GtkWidget *add_button;
GtkWidget *delete_button;
GtkWidget *edit_button;
GtkWidget *about_button;
GtkWidget *read_button;
GtkEntryCompletion *completion;
//------------------------------------------------------------
// cac doi tuong o muc add
GtkWidget *dialog_add;
GtkWidget *button_ok;
GtkWidget *button_cancle_add;
GtkWidget *entry_add;
GtkTextView *textview_add;
//-----------------------------------------------------------
//cac doi tuong o muc delete
GtkDialog *dialog_delete;
GtkWidget *button_delete;
GtkWidget *button_cancle_delete;
GtkWidget *entry_delete;
//-------------------------------------------------------------
//cac doi tuong o muc read
GtkDialog *dialog_read;
GtkWidget *button_read;
GtkWidget *button_cancle_read;
GtkWidget *entry_read;
//-------------------------------------------------------------
//cac doi tuong o muc edit
GtkDialog *dialog_edit;
GtkWidget *entry_edit;
GtkTextView *textview_edit;
GtkWidget *button_edit;
GtkWidget *button_cancle_edit;
//------------------------------------------------------------------
GtkDialog *aboutdialog;

//----------------------------------------------------------------------------------------------------------
void on_window_destroy(GObject *object,gpointer user_data);// nhan x(tat chuong trinh)
void on_button_find_ev_clicked(GObject *object,gpointer user_data); // nhan find
void on_button_add_ev_clicked(GObject *object,gpointer user_data);// nhan add
void on_button_delete_ev_clicked(GObject *object,gpointer user_data);// nhan delete
void on_button_edit_ev_clicked(GObject *object,gpointer user_data);// nhan edit
void on_button_read_ev_clicked(GObject *object,gpointer user_data);// nhan read
void on_button_about_ev_clicked(GObject *object,gpointer user_data);// nhan about
void on_button_ok_clicked(GObject *object,gpointer user_data);// nhan nut ok cua add
void on_button_cancle_add_clicked(GObject *object,gpointer user_data);// nhan cancle cua add
void on_button_delete_clicked(GObject *object,gpointer user_data);// nhan nut delete cua delete
void on_button_cancle_delete_clicked(GObject *object,gpointer user_data);// nhan cancle cua delete
void on_button_edit_clicked(GObject *object,gpointer user_data); // nhan ok cua edit
void on_button_cancle_edit_clicked(GObject *object,gpointer user_data);// nhan cancle cua edit
gboolean on_enter_key_press_event(GtkWidget *widget,GdkEventKey *pKey,gpointer user_data);//nhan enter
gboolean on_tab_key_press_event(GtkWidget *widget,GdkEventKey *pKey,gpointer user_data);//nhan tab
void match_select(GtkEntryCompletion *widget,GtkTreeModel *model,GtkTreeIter *iter,gpointer user_data);//hoan thanh tu khi click
void setup_list();//add tu dien vao danh sach hien thi
//----------------------------------------------------------------------------------------------------------
void on_window_destroy(GObject *object,gpointer user_data)
{
  btcls(evdata);
  g_print("\n--------------------Thanks for using-----------------------\n.......................Quitting..........................\n");
  gtk_main_quit();
}
void on_button_find_ev_clicked(GObject *object,gpointer user_data)
{
  const gchar *word;
  char data[100000];
  char sou[100];
  char mean[100000],*soun;
  int i,j=0;
  GtkTextBuffer *buff;
  if(!ok){
    buff = gtk_text_view_get_buffer(mean_textview); // Returns the GtkTextBuffer being displayed by this text view.
    gtk_text_buffer_set_text(buff,"\n     ===> ERROR: Tu dien chua co du lieu! <===",strlen("\n     ===> ERROR: Tu dien chua co du lieu! <==="));//xoa dong noi dung cua buff va insert cai kia
    return;
  }
  word = gtk_entry_get_text(GTK_ENTRY(find_entry));//lay tu o cho tim kiem
  if(strcmp(word,"")==0){ //kiem tra xem nhap chua
    buff = gtk_text_view_get_buffer(mean_textview); // Returns the GtkTextBuffer being displayed by this text view.
    gtk_text_buffer_set_text(buff,"\n     ===> ERROR: No word is entered! <===",strlen("\n     ===> ERROR: No word is entered! <==="));
    return;
  }
  if(bfndky(evdata,(char *)word,&i) == 0){
    if(clock_getres(CLOCK_REALTIME, &ts) == 0)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        btsel(evdata,(char*)word,data,1000,&i);
        clock_gettime(CLOCK_REALTIME, &end);
        printf("time for this algorithm = %u nanoseconds\n", (unsigned int)((end.tv_sec - end.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)));
        printf("\n");
    }
    else
    {
        printf("clock realtime is not supported\n");
    }
    strcpy(mean,data);
    buff = gtk_text_view_get_buffer(mean_textview); //Returns the GtkTextBuffer being displayed by this text view
    gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));//xoa dong noi dung cua buff va insert cai kia
    return ;
  }
  else{
    sound = btopn("tdanhvietsoundex",0,0);
    soundex((char*)word,sou);
    if(bfndky(sound,(char*)sou,&i)==0){
      btsel(sound,sou,mean,1000,&i);
      buff = gtk_text_view_get_buffer(mean_textview); //Returns the GtkTextBuffer being displayed by this text view
      gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));
      btcls(sound);
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void on_button_ok_clicked(GObject *object,gpointer user_data){//add tu moi{
  GtkTextBuffer *buff;
  GtkWidget *message;
  GtkTextIter first,last;
  const gchar *word;
  const gchar *mean;
  int i;
  g_assert(GTK_IS_ENTRY(entry_add));
  word = gtk_entry_get_text(GTK_ENTRY(entry_add));//lay chuoi tu entry_add
  buff = gtk_text_view_get_buffer(textview_add);//lay chuoi tu textview_add
  gtk_text_buffer_get_start_iter(buff,&first);//
  gtk_text_buffer_get_end_iter(buff,&last);
  mean = gtk_text_buffer_get_text(buff,&first,&last,FALSE);//mean lay chuoi tu first den last
  if(strcmp(word,"")==0)return;
  else
    if(bfndky(evdata,(char*)word,&i)==0){
      message = gtk_message_dialog_new(GTK_WINDOW(dialog_add),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"This word  existed!","Info");
      gtk_window_set_title(GTK_WINDOW(message),"Information");
      gtk_dialog_run(GTK_DIALOG(message));
      gtk_widget_destroy(message);
    }
    else{
      if(clock_getres(CLOCK_REALTIME, &ts) == 0)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        btins(evdata,(char*)word,(char*)mean,1000);// insert
        clock_gettime(CLOCK_REALTIME, &end);
        printf("time for this algorithm = %u nanoseconds\n", (unsigned int)((end.tv_sec - end.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)));
        printf("\n");
        ok=1;
    }
    else
    {
        printf("clock realtime is not supported\n");
    }
      sound = btopn("tdanhvietsoundex",0,0);
      sound = xulysoundex(sound,(char*)word);
      btcls(sound);
      message = gtk_message_dialog_new(GTK_WINDOW(dialog_add),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Completed !","Info");
      gtk_window_set_title(GTK_WINDOW(message),"Information");
      gtk_dialog_run(GTK_DIALOG(message));
      gtk_widget_destroy(message);
    }
  gtk_entry_set_text(GTK_ENTRY(entry_add),"");
  buff = gtk_text_view_get_buffer(textview_add);
  gtk_text_buffer_set_text(buff,"",1);
  setup_list();//cai tu dien vao danh sach hien thi
}
void on_button_add_ev_clicked(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  gtk_entry_set_text(GTK_ENTRY(find_entry),"");//thay doi du lieu cua find_entry
  buff = gtk_text_view_get_buffer(mean_textview);
  gtk_text_buffer_set_text(buff,"",1);//xoa dong noi dung cua buff va insert cai kia
  gtk_dialog_run(GTK_DIALOG(dialog_add));//chay diolog
  gtk_widget_hide(GTK_WIDGET(dialog_add));//
}
void on_button_cancle_add_clicked(GObject *object,gpointer user_data){
  gtk_widget_hide(GTK_WIDGET(dialog_add));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void on_button_edit_ev_clicked(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  gtk_entry_set_text(GTK_ENTRY(find_entry),"");
  buff = gtk_text_view_get_buffer(mean_textview);
  gtk_text_buffer_set_text(buff,"",1);
  gtk_dialog_run(GTK_DIALOG(dialog_edit));
  gtk_widget_hide(GTK_WIDGET(dialog_edit));
}
void on_button_edit_clicked(GObject *object,gpointer user_data){
  const gchar *word;
  const gchar *mean;
  GtkTextBuffer *buff;
  GtkTextBuffer *buff1;
  int i;
  GtkWidget *message;
  GtkTextIter first,last;
  g_assert(GTK_IS_ENTRY(entry_edit));
  word = gtk_entry_get_text(GTK_ENTRY(entry_edit));//lay chuoi tu entry_edit
  buff = gtk_text_view_get_buffer(textview_edit);//lay buff tu textview_edit(kieu nhu lay doi tuog)
  gtk_text_buffer_get_start_iter(buff,&first);//khoi tao first len vi tri dau cua buff
  gtk_text_buffer_get_end_iter(buff,&last);//khoi tao last o vi tri cuoi cua buff
  mean = gtk_text_buffer_get_text(buff,&first,&last,FALSE);//Returns the text in the range [start ,end )
  if(strcmp(word,"")==0)return;
  if(bfndky(evdata,(char*)word,&i)==0){
    if(clock_getres(CLOCK_REALTIME, &ts) == 0)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        btupd(evdata,(char*)word,(char*)mean,1000);//sua lai val cho key
        clock_gettime(CLOCK_REALTIME, &end);
        printf("time for this algorithm = %u nanoseconds\n", (unsigned int)((end.tv_sec - end.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)));
        printf("\n");
    }
    else
    {
        printf("clock realtime is not supported\n");
    }
    sound = btopn("tdanhvietsoundex",0,0);
    sound = xulysoundex(sound,(char*)word);
    btcls(sound);
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_edit),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Completed!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  else{
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_edit),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"This word not exist!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  gtk_entry_set_text(GTK_ENTRY(entry_edit),"");
  buff1 = gtk_text_view_get_buffer(textview_edit); //lay chuoi cua textview_add
  gtk_text_buffer_set_text(buff,"",1);//gan == NULL
  setup_list();
}
void on_button_cancle_edit_clicked(GObject *object,gpointer user_data){
   gtk_widget_hide(GTK_WIDGET(dialog_edit));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void on_button_delete_ev_clicked(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  gtk_entry_set_text(GTK_ENTRY(find_entry),"");
  buff = gtk_text_view_get_buffer(mean_textview);
  gtk_text_buffer_set_text(buff,"",1);
  gtk_dialog_run(GTK_DIALOG(dialog_delete));
  gtk_widget_hide(GTK_WIDGET(dialog_delete)); //khi chay xong phai an
}

void on_button_delete_clicked(GObject *object,gpointer user_data){
  const gchar *word;
  GtkWidget *message;
  int i;
  word = gtk_entry_get_text(GTK_ENTRY(entry_delete));//lay tu o entry_delete
  printf("%s\n",word);
  if(bfndky(evdata,(char*)word,&i)==0){ // kiem tra cai evdata
    if(clock_getres(CLOCK_REALTIME, &ts) == 0)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        btdel(evdata,(char*)word); //xoa trong evdata
        clock_gettime(CLOCK_REALTIME, &end);
        printf("time for this algorithm = %u nanoseconds\n", (unsigned int)((end.tv_sec - end.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)));
        printf("\n");
    }
    else
    {
        printf("clock realtime is not supported\n");
    }
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Delete Success!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
    printf("Delete success!!!\n");
  }
  else{//khi k co tu
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"this word not exist!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  gtk_entry_set_text(GTK_ENTRY(entry_delete),"");//sau khi nhap xong no van luu o entry_delete la tu, nen phai gan no == ""
  setup_list();
}
void on_button_cancle_delete_clicked(GObject *object,gpointer user_data){
  gtk_widget_hide(GTK_WIDGET(dialog_delete));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void on_button_read_ev_clicked(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  gtk_entry_set_text(GTK_ENTRY(find_entry),"");
  buff = gtk_text_view_get_buffer(mean_textview);
  gtk_text_buffer_set_text(buff,"",1);
  gtk_dialog_run(GTK_DIALOG(dialog_read));
  gtk_widget_hide(GTK_WIDGET(dialog_read));
}
void on_button_cancle_read_clicked(GObject *object,gpointer user_data){
  gtk_widget_hide(GTK_WIDGET(dialog_read));
}

void on_button_read_clicked(GObject *object,gpointer user_data){
  const gchar *namefile;
  GtkWidget *message;
  FILE *f;
  char s[1000],key[1000],val[1000];
  int i=0,j=0;
  namefile = gtk_entry_get_text(GTK_ENTRY(entry_read));
  f = fopen(namefile,"r");
  if(f==NULL){
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"File ERROR!!!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
    gtk_widget_hide(GTK_WIDGET(dialog_read));
    return;
  }
  btcls(evdata);
  BTA *p = btcrt("tdanhviet", 0, 0);
  BTA *q = btcrt("tdanhvietsoundex", 0,0);
  while(fgets(s,1000,f)!=NULL){
    if ((s[0]=='/'&&s[1]=='/')||s[0]=='['||s[0]==']'||strlen(s)<6) continue;
    s[strlen(s)-1] = '\0';
    //printf("%d\t%s\n",strlen(s),s);
    i=j=0;
    while(s[i++]!='{');
    while(s[i]!='}')key[j++] = s[i++];
    key[j]='\0';
    j = 0;
    while(s[i++]!='{');
    while(s[i]!='}')val[j++] = s[i++];
    val[j]='\0';
    btins(p, key, val, 10000);
    q = xulysoundex(q,key);
    //puts(key);
    //puts(val);
  }
  btcls(p);
  btcls(q);
  fclose(f);
  evdata = btopn("tdanhviet",0,0);
  message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Read Success!","Info");
  gtk_window_set_title(GTK_WINDOW(message),"Information");
  gtk_dialog_run(GTK_DIALOG(message));
  gtk_widget_destroy(message);
  gtk_widget_hide(GTK_WIDGET(dialog_read));
  setup_list();
  ok=1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void on_button_about_ev_clicked(GObject *object,gpointer user_data){
  gtk_dialog_run(aboutdialog);
  gtk_widget_hide(GTK_WIDGET(aboutdialog));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

gboolean on_enter_key_press_event(GtkWidget *widget,GdkEventKey *pKey,gpointer user_data){//khi an enter
  char data[1000];
  char s[1000];
  char *so;
  int i;
  int k = 0;
  if(pKey){
    if(pKey->keyval == GDK_KEY_Return)
      on_button_find_ev_clicked(G_OBJECT(widget),user_data);
  }
  return FALSE;
}
gboolean on_tab_key_press_event(GtkWidget *widget,GdkEventKey *pKey,gpointer user_data){
  const gchar *word;
  int d,size;
  char s1[1000],s2[10000];
  if(pKey){
    if(pKey->keyval == GDK_KEY_Tab){
      word = gtk_entry_get_text(GTK_ENTRY(find_entry));
      btpos(evdata,1);
      while((d = btseln(evdata,s1,s2,10000,&size))==0)
        if(strncmp((char*)word,s1,strlen((char*)word))==0){//so sanh strlen((char*)word) ki tu dau cua word voi s1
          gtk_entry_set_text(GTK_ENTRY(find_entry),s1);
          break;
        }
    }
  return FALSE;
  }
}
void match_select(GtkEntryCompletion *widget,GtkTreeModel *model,GtkTreeIter *iter,gpointer user_data){
  GValue value = {0, };
  const char *word;
  gtk_tree_model_get_value(model,iter,COL_WORD,&value);//khoi tao va thiet lap value vao COL_WORD
  word = g_value_get_string(&value);//lay noi dung chuoi trong value
  gtk_entry_set_text(GTK_ENTRY(find_entry),word);
  g_value_unset(&value);// xoa gia tri va unset kieu du lieu
}
void setup_list()
{

  GtkListStore *liststore;
  GtkTreeIter iter;
  char word[1000];
  char data[1000];
  char mean[1000];
  int i;
  completion = gtk_entry_completion_new(); //creat a new GtkEntryCompletion
  gtk_entry_completion_set_text_column(completion,COL_WORD);//ham cai dat completion de hien thi cac chuoi trong danh sach,lay cac chuoi tu column trong cac mau completion
  gtk_entry_set_completion(GTK_ENTRY(find_entry),completion);
  g_signal_connect(G_OBJECT(completion),"match-selected",G_CALLBACK(match_select),NULL);//ket noi tin hieu
  liststore = gtk_list_store_new(1,G_TYPE_STRING);
  btpos(evdata,1);
  while(btseln(evdata,word,data,1000,&i)==0){
    gtk_list_store_append(liststore,&iter);
    gtk_list_store_set(liststore,&iter,COL_WORD,word,-1);
  }
  gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));//neu completion da duoc cai dat, no se remove truoc khi cai dat cai moi
}

int main(int argc,char *argv[]){
  if((evdata=btopn("tdanhviet",0,0))==NULL) {
    ok=0;
    evdata = btcrt("tdanhviet",0,0);
    BTA *q = btcrt("tdanhvietsoundex", 0,0);
    btcls(q);
  }
  GError *error = NULL;
  gtk_init(&argc,&argv);//bat buoc phai co khi co giao dien do hoa
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder,"dic.glade",&error);
  if(error != NULL){ //kiem tra xem co build dc k
	  g_warning("%s",error->message);
    g_error_free(error);
	  return 1;
  }
  //lien ket cac bien voi id
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_interface"));
  combobox = GTK_WIDGET(gtk_builder_get_object(builder, "combobox"));
  find_entry = GTK_WIDGET(gtk_builder_get_object(builder, "word_entry"));
  word_treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview_ev"));
  find_button = GTK_WIDGET(gtk_builder_get_object(builder, "find_button"));
  add_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_add_ev"));
  delete_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_delete_ev"));
  edit_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_edit_ev"));
  read_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_read_ev"));
  about_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_about_ev"));
  mean_textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview_ev"));

  dialog_add = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_add"));
  button_ok = GTK_WIDGET(gtk_builder_get_object(builder, "button_ok"));
  button_cancle_add = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancle_add"));
  entry_add = GTK_WIDGET(gtk_builder_get_object(builder, "entry_add_word"));
  textview_add = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview_add"));

  dialog_delete = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_delete"));
  button_delete = GTK_WIDGET(gtk_builder_get_object(builder, "button_delete"));
  button_cancle_delete = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancle_delete"));
  entry_delete = GTK_WIDGET(gtk_builder_get_object(builder, "entry_delete"));

  dialog_edit = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_edit"));
  button_edit = GTK_WIDGET(gtk_builder_get_object(builder, "button_edit"));
  button_cancle_edit = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancle_edit"));
  entry_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_edit"));
  textview_edit = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview_edit"));

  dialog_read = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_read"));
  button_read = GTK_WIDGET(gtk_builder_get_object(builder, "button_read"));
  button_cancle_read = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancle_read"));
  entry_read = GTK_WIDGET(gtk_builder_get_object(builder, "entry_read"));


  aboutdialog = GTK_DIALOG(gtk_builder_get_object(builder, "aboutdialog"));
  // ket noi cac tin hieu
  gtk_builder_connect_signals(builder,NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window_destroy), NULL);
  g_signal_connect(G_OBJECT(find_entry), "key_press_event", G_CALLBACK(on_enter_key_press_event), NULL);
  g_signal_connect(G_OBJECT(find_entry), "key_press_event", G_CALLBACK(on_tab_key_press_event), NULL);
  g_signal_connect(G_OBJECT(find_button), "clicked", G_CALLBACK(on_button_find_ev_clicked), NULL);


  g_signal_connect(G_OBJECT(button_read), "clicked", G_CALLBACK(on_button_read_clicked),NULL);
  g_signal_connect(G_OBJECT(button_cancle_read), "clicked", G_CALLBACK(on_button_cancle_read_clicked), NULL);


  g_signal_connect(G_OBJECT(add_button), "clicked", G_CALLBACK(on_button_add_ev_clicked), NULL);
  g_signal_connect(G_OBJECT(delete_button), "clicked", G_CALLBACK(on_button_delete_ev_clicked), NULL);
  g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(on_button_edit_ev_clicked), NULL);
  g_signal_connect(G_OBJECT(read_button), "clicked", G_CALLBACK(on_button_read_ev_clicked), NULL);
  g_signal_connect(G_OBJECT(about_button), "clicked", G_CALLBACK(on_button_about_ev_clicked), NULL);
  g_signal_connect(G_OBJECT(button_ok), "clicked", G_CALLBACK(on_button_ok_clicked), NULL);
  g_signal_connect(G_OBJECT(button_cancle_add), "clicked", G_CALLBACK(on_button_cancle_add_clicked), NULL);
  g_signal_connect(G_OBJECT(button_delete), "clicked", G_CALLBACK(on_button_delete_clicked),NULL);
  g_signal_connect(G_OBJECT(button_cancle_delete), "clicked", G_CALLBACK(on_button_cancle_delete_clicked), NULL);
  g_signal_connect(G_OBJECT(button_edit), "clicked", G_CALLBACK(on_button_edit_clicked), NULL);
  g_signal_connect(G_OBJECT(button_cancle_edit), "clicked", G_CALLBACK(on_button_cancle_edit_clicked), NULL);

  g_object_unref(G_OBJECT(builder));// kieu nhu dong file
  setup_list();
  gtk_widget_show(window);//hien window
  gtk_main();
  return 0;
}
