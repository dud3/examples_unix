#include <stdlib.h>
#include <string.h>

#include "clist.h"


void clist_init(CList *list, void (*destroy)(void *data)) 
{
   list->size = 0;
   list->destroy = destroy;
   list->head = NULL;

   return;
}


int clist_ins_next(CList *list, CListElmt *element, const void *data) 
{
   CListElmt *new_element;

   if ((new_element = (CListElmt *)malloc(sizeof(CListElmt))) == NULL)
      return -1;

   new_element->data = (void *)data;

   if (clist_size(list) == 0) {
      new_element->next = new_element;
      list->head = new_element;
   }

   else {
      
      // Handle insertion when the list is not empty.                           
      new_element->next = element->next;
      element->next = new_element;
   }

   // Adjust the size of the list to account for the inserted element.          *
   list->size++;

   return 0;
}


int clist_rem_next(CList *list, CListElmt *element, void **data) {

   CListElmt *old_element;

   if (clist_size(list) == 0)
      return -1;

   *data = element->next->data;

   if (element->next == element) {
      // Handle removing the last element.
      old_element = element->next;
      list->head = NULL;
   }
   else {
      // Handle removing other than the last element.
      old_element = element->next;
      element->next = element->next->next;
   }

   free(old_element);

   list->size--;

   return 0;
}


void clist_destroy(CList *list) 
{
   void *data;

   while (clist_size(list) > 0) {
      
      // Why are we casting here, isn't data undefined/uninitialized?
      // couldn't we just define void **data instead of void *data above?
      // and pass it as data instead of casting the memory address of a pointer
      // to dobule pointer to void?
      if (clist_rem_next(list, list->head, (void **)&data) == 0 && list->destroy
         != NULL) {
         
         // Call a user-defined function to free dynamically allocated data.    *
         list->destroy(data);
      }
   }

   // No operations are allowed now, but clear the structure as a precaution.
   memset(list, 0, sizeof(CList));

   return;
}
