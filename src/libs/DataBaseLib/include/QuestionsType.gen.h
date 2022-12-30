/************************************************************************************************
** File created by QxEntityEditor 1.2.6 (2022/12/29 03:00) : please, do NOT modify this file ! **
************************************************************************************************/

#ifndef _BRIDGESERVER_QUESTIONSTYPE_H_
#define _BRIDGESERVER_QUESTIONSTYPE_H_

#ifdef _QX_NO_PRECOMPILED_HEADER
#ifndef Q_MOC_RUN
#include "../include/bridgeServer_precompiled_header.gen.h" // Need to include precompiled header for the generated moc file
#endif // Q_MOC_RUN
#endif // _QX_NO_PRECOMPILED_HEADER

class BRIDGESERVER_EXPORT QuestionsType
{

public:

   enum enum_QuestionsType
   {
      FIRST_PHONE_NUMBER = 0,
      FAVOURITE_TEACHER = 1,
      FAVOURITE_SINGER = 2,
      MOTHERS_NAME = 3
   };

private:

   QuestionsType();
   virtual ~QuestionsType();

};

#endif // _BRIDGESERVER_QUESTIONSTYPE_H_
