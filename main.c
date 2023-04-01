#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Siehe Angabe

enum node_type
{
    number_type = 1, operator_type, parenthesis_type
};

//Siehe Angabe

struct node
{
    char operator;
    int number;
    enum node_type type;
    struct node* next;
};

//Funktionsprototypen

struct node* inputToNode(const char *input);

struct node* createParenthesisNode(char operator);
struct node* createOperatorNode(char operator);
struct node* createNumberNode(int number);
struct node* addLast(struct node *head, struct node *newNode);
struct node* findLastParenthesisOpen(struct node *head);
struct node* findFirstParenthesisClosing(struct node *parenthesis);
struct node* findFirstPointOperator(struct node *startNode, struct node *endNode);
struct node* findFirstDashOperator(struct node *startNode, struct node *endNode);
struct node* findPrevious(struct node *startNode, struct node *node);
struct node* calculate(struct node *head);
void printCalculation(struct node* head);

//Main Funktion

int main()
{
    struct node* head = NULL;
    char input[50];

    //Schleife zur Eingabe der Rechnung
    do
    {
        printf("\nInput: ");
        scanf("%s", input);
        if(input[0] == '=')
        {
            break;                                      //Wenn Eingabe '=' ist, dann break aus der while Schleife raus, damit das '=' nicht im Struct gespeichert wird
        }
        head = addLast(head, inputToNode(input));       //
        printf("\nTerm: ");
        printCalculation(head);                         //Funktion zur Ausgabe der Rechnung (nach jeder Eingabe erforderlich)
    }
    while(input[0] != '=');

    calculate(head);                                    //Funktion zum Ausrechnen nach beendeter Eingabe

    free(head);                                         //Speicher vom Head (Endergebnis der Rechnung) freisetzen

    return 0;
}

//-----Funktionen zur Eingabe-----------------------------------------------------------------------------------------------------------------------------------------------------------

//Siehe Angabe

struct node *inputToNode(const char *input)
{
    int number = atoi(input);
    if (number != 0)
    {
        return createNumberNode(number);
    }
    if (input[0] == '0')
    {
        return createNumberNode(0);
    }
    if (strcmp(input, "(") == 0 ||
            strcmp(input, ")") == 0)
    {
        return createParenthesisNode(input[0]);
    }

    if (strcmp(input, "+") == 0 ||
            strcmp(input, "-") == 0 ||
            strcmp(input, "*") == 0 ||
            strcmp(input, "/") == 0)
    {
        return createOperatorNode(input[0]);
    }
    return NULL;
}

//Neue Klammer in die Liste einf�gen

struct node* createParenthesisNode(char operator)
{
    struct node* newNode =(struct node*)malloc(sizeof(struct node));    //Speicherplatz kreieren
    newNode->operator = operator;                                       //Operator(char) des Knotens mit der einegebenen Klammer gleichsetzen
    newNode->type = operator_type;                                      //Typ des Knotens gleich operator_type setzen (enum)
    return newNode;                                                     //Knoten zur�ckgeben
};

//Neuen Rechenoperator in die Liste einf�gen

struct node* createOperatorNode(char operator)
{
    struct node* newNode =(struct node*)malloc(sizeof(struct node));    //Speicherplatz schaffen
    newNode->operator = operator;                                       //Operator(char) des Knotens mit eingegebenen Rechenoperator gleichsetzen
    newNode->type = operator_type;                                      //Typ des Knotens gleich operator_type setzen
    return newNode;                                                     //Knoten zur�ckgeben
};

//Neue Zahl in die Liste einf�gen

struct node* createNumberNode(int number)
{
    struct node* newNode =(struct node*)malloc(sizeof(struct node));    //Speicherplatz schaffen
    newNode->number = number;                                           //Nummer(int) des Knotens mit eingegebener Nummer gleichsetzen
    newNode->type = number_type;                                        //Typ des Knotens gleich number_type setzen
    return newNode;                                                     //Knoten zur�ckgeben
};

//Funktion um neuen Knoten am Ende der Liste einzuf�gen

struct node* addLast(struct node *head, struct node *newNode)           //Head und eingegebener Operator/Zahl wird �bergeben
{
    if (head == NULL)                                                   //Wenn die Liste noch leer ist (head == NULL)...
    {
        newNode->next = head;                                           //...n�chstn Knoten nach eingegebener Zahl/Operator NULL(== head) setzen
        return newNode;                                                 //Neuen Knoten zur�ckgeben (Wird in Main dem head gleichgesetzt)
    }

    struct node* temp = head;                                           //Tempor�rer Knoten um die Liste in einer Schleife durchzugehen
    while(temp->next != NULL)                                           //Liste bis zum Ende durchgehen
    {
        temp = temp->next;
    }
    temp->next = newNode;                                               //Letzten Knoten der Liste mit neu eingegebenem Operator/Zahl gleichsetzen
    newNode->next = NULL;                                               //Den Knoten danach NULL setzen
    return head;                                                        //Den head der Liste zur�ckgeben (Wird in der Main dem head gleichgesetzt)
};

//-----Funktionen zum Rechnen----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct node* calculate(struct node *head)                                       //head der Liste wird �bergeben
{
    while(head->next != NULL)                                                   //Schleife um die ganze Liste bis zum Ende durchzugehen
    {
        //Variablen deklarieren und initialisieren
        struct node* parenthesis = head;                                        //Parenthesis wird dem head gleichgesetzt
        struct node* closingparenthesis = NULL;                                 //alle anderen Variablen werden NULL gesetzt
        struct node* previous = NULL;
        struct node* operator = NULL;

        if(findLastParenthesisOpen(parenthesis) != NULL)                        //Wenn eine ge�ffnete Klammer gefunden wird...
        {
            parenthesis = findLastParenthesisOpen(parenthesis);                 //Parenthesis wird der letzten ge�ffneten Klammer gleichgesetzt
            closingparenthesis = findFirstParenthesisClosing(parenthesis);      //Closingparenthesis wird der ersten geschlossenen Klammer nach Parenthesis gleichgesetzt
        }
        if(findFirstPointOperator(parenthesis, closingparenthesis) != NULL)     //Ersten Punkt-Operator finden (wenn Parenthesis == head -> wird die ganze Liste durchsucht; wenn Parenthesis == Knoten mit der letzten Ge�ffneten Klammer -> wird nur zwischen Parenthesis und Closingparenthesis gesucht)
        {                                                                       //Wenn ein Punkt-Operator gefunden wurde...
            operator = findFirstPointOperator(parenthesis, closingparenthesis); //... Operator wird gleich dem gefundenen Operator gesetzt...
            previous = findPrevious(parenthesis, operator);                     //... Previous wird gleich der Zahl vor dem Operator gesetzt
            if(operator->operator == '*')                                       //Je nach Operator wird multipliziert oder dividiert
            {
                previous->number = previous->number * operator->next->number;
            }
            else if (operator->operator == '/')
            {
                if(operator->next->number == 0)
                {
                    operator->next->number = 1;
                }
                previous->number = previous->number / operator->next->number;
            }
        }
        else if(findFirstDashOperator(parenthesis, closingparenthesis) != NULL) //Wenn kein Punkt-Operator gefunden wurde, wird nach dem gleichen Prinzip mit Strich-Operatoren durchgegangen und gerechnet
        {
            operator = findFirstDashOperator(parenthesis, closingparenthesis);
            previous = findPrevious(parenthesis, operator);
            if(operator->operator == '+')
            {
                previous->number = previous->number + operator->next->number;
            }
            else if(operator->operator == '-')
            {
                previous->number = previous->number - operator->next->number;
            }
        }

        //Aufl�sen der Klammern & free

        if(findLastParenthesisOpen(parenthesis) != NULL)                            //Wenn in der Rechnung eine ge�ffnete Klammer vorkommt...
        {
            previous->next = operator->next->next;                                  //Knoten nach Previous wird gleich dem Knoten von Closingparenthesis gesetzt
            free(operator->next);                                                   //Knoten nach dem Operator wird gefreet
            free(operator);                                                         //Knoten des Operators wird gefreet

            printf("\nResulting term: ");
            printCalculation(head);                                                 //Ausgabe des Zwischenergebnisses mit Klammern

            previous->next = closingparenthesis->next;                              //Knoten nach Previous wird gleich dem Knoten nach Closingparenthesis gesetzt
            free(closingparenthesis);                                               //Closingparenthesis wird gefreet
            if(parenthesis != head)                                                 //Wenn Parenthesis nicht am Anfang der Liste steht...
            {
                struct node* previousoperator = findPrevious(head, parenthesis);    //...Neue Variable Previousoperator wird gleich dem Operator vor Parenthesis gesetzt
                previousoperator->next = parenthesis->next;                         //Knoten nach Previousoperator wird gleich dem Knoten nach Parenthesis (== Previous) gesetzt
                free(parenthesis);                                                  //Parenthesis wird gefreet
            }
            else                                                                    //Wenn Parenthesis am Anfang der Liste steht...
            {
                head = previous;                                                    //...wird head gleich Previous gesetzt
            }
        }
        else                                                                        //Wenn in der Rechnung keine Klammer vorhanden ist...
        {
            previous->next = operator->next->next;                                  //...wird der Knoten nach Previous dem Knoten nach dem Knoten von Operator gesetzt...
            free(operator->next);                                                   //...der Knoten nach dem Operator wird gefreet...
            free(operator);                                                         //...und der Operator wird gefreet

            printf("\nResulting term: ");
            printCalculation(head);                                                 //Ausgabe des Zwischenergebnisses oder des Endergebnisses wenn nur noch der head �brig ist
        }

    }
    return head;                                                                    //Nach durchlaufen und berechnen der ganzen Liste wird der head (== Endergebnis) zur�ckgegeben & in der Main gefreet
};

//Letzte ge�ffnete Klammer finden

struct node* findLastParenthesisOpen(struct node *head)
{
    struct node* parenthesis = NULL;                                                //Variable zum Speichern des Knotens der eine ge�ffnete Klammer enth�lt
    struct node* temp = head;                                                       //Tempor�re Variable zum Durchlaufen der gesamten Liste
    while(temp != NULL)                                                             //Schleife zum Durchlaufen der Liste BIS ZUM ENDE
    {
        if(temp->operator == '(')                                                   //Wenn eine ge�ffnete Klammer gefunden wurde...
        {
            parenthesis = temp;                                                     //Knoten Parenthesis wird dem tempor�ren Knoten der die Klammer enth�lt gleichgesetzt
        }
        temp = temp->next;
    }
    return parenthesis;                                                             //Letzte ge�ffnete Klammer wird zur�ckgegeben (Wenn es keine Klammer gibt wird NULL zur�ckgegeben)
};

//Erste geschlossene Klammer nach der letzten ge�ffneten Klammer finden

struct node* findFirstParenthesisClosing(struct node *parenthesis)
{
    struct node* temp = parenthesis;                                                //Tempor�re Variable wird dem Knoten der �bergebenen letzten ge�ffneten Klammer gleichgesetzt
    while(temp != NULL)                                                             //Liste wird bis zum Ende durchlaufen
    {
        if(temp->operator == ')')                                                   //Wenn eine geschlossene Klammer gefunden wird...
        {
            return temp;                                                            //...wird der Knoten der geschlossenen Klammer gleichgesetzt und zur�ckgegeben
        }
        temp = temp->next;
    }
    return NULL;                                                                    //Wenn keine geschlossene Klammer gefunden wurde wird NULL zur�ckgegeben
};

//Ersten Punkt-Operator finden

struct node *findFirstPointOperator(struct node *startNode, struct node *endNode)   //Wenn es Klammern gibt werden Parenthesis und Closingparenthesis �bergeben, wenn nicht werden head und NULL �bergeben
{
    struct node* temp = startNode;                                                  //Tempor�re Variable wird gleich Parenthesis/head gesetzt
    while(temp->next != endNode)                                                    //Schleife zum durchlaufen der Liste bis Closingparenthesis/NULL
    {
        if(temp->operator == '*' || temp->operator == '/')                          //Wenn ein Punkt-Operator gefunden wird...
        {
            return temp;                                                            //...wird dieser returned
        }
        temp = temp->next;
    }
    return NULL;                                                                    //Wenn kein Punkt-Operator gefunden wird, wird NULL returned
};

//Ersten Strich-Operator finden

struct node* findFirstDashOperator(struct node *startNode, struct node *endNode)    //Gleiches Prinzip wie beim Dash-Operator finden
{
    struct node* temp = startNode;
    while(temp->next != endNode)
    {
        if(temp->operator == '+' || temp->operator == '-')
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
};

//Knoten vor einem anderen Knoten finden

struct node* findPrevious(struct node *startNode, struct node *node)
{
    struct node* temp = startNode;                                      //Tempor�re Variable zum Durchlaufen der Liste

    while(temp != NULL)                                                 //Durchlaufen der Liste bis zum Ende
    {
        if(temp->next==node)                                            //Wenn der Knoten nach dem tempor�ren Knoten der �bergebenen Node entspricht...
        {
            return temp;                                                //...wird der tempor�re Knoten �bergeben
        }
        temp = temp->next;
    }
    return NULL;                                                        //Wenn es keinen Knoten vor dem �bergebenen gibt, wird NULL zur�ckgegeben
};

//Funktion zur Ausgabe der Liste

void printCalculation(struct node* head)
{
    struct node* temp = head;                                           //Tempor�re Variable zum Durchlaufen der Liste
    while(temp != NULL)                                                 //Schleife zum Durchlaufen der Liste
    {
        if(temp->type == number_type)                                   //Wenn der Typ des tempor�ren Knotens eine Zahl ist, wird diese ausgegeben
        {
            printf("%d", temp->number);
        }
        else                                                            //Wenn der Typ des tempor�ren Knotens ein Operator ist, wird dieser ausgegeben
        {
            printf("%c", temp->operator);
        }
        temp = temp->next;
    }
}
