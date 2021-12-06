#ifndef ME_H
#define	ME_H

struct fsmT // Finite State Machine
{
    void *(*func)(void * arg, void * next);
    void * arg;
    void * next;
    
};



#endif	/* ME_H */
