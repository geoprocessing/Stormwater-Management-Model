#include <stdio.h>
#include "com_geoprocessing_model_swmm_JSWMM.h"
#include "swmm5.h"
#include <string.h>
#include "toolkitAPI.h"
# include <stdlib.h>

jstring charToJstring(JNIEnv *env, const char *pat) {
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>","([BLjava/lang/String;)V");
    jbyteArray bytes = (*env)->NewByteArray(env, strlen(pat));
    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(pat), (jbyte *) pat);
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, (*env)->NewStringUTF(env, "utf-8"));
}


/*
 * refer to swmm_run
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_run
	(JNIEnv * env, jobject jobj, jstring jinfile, jstring jrptfile, jstring joutfile){
	const char* infile = (*env)->GetStringUTFChars(env, jinfile, NULL);
	const char* rptfile = (*env)->GetStringUTFChars(env, jrptfile, NULL);
	const char* outfile = (*env)->GetStringUTFChars(env, joutfile, NULL);
	return swmm_run((char*)infile,(char*)rptfile,(char*)outfile);
}

/*
 * refer to swmm_open
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_open
	(JNIEnv * env, jobject jobj, jstring jinfile, jstring jrptfile, jstring joutfile){
	const char* infile = (*env)->GetStringUTFChars(env, jinfile, NULL);
	const char* rptfile = (*env)->GetStringUTFChars(env, jrptfile, NULL);
	const char* outfile = (*env)->GetStringUTFChars(env, joutfile, NULL);
	return swmm_open((char*)infile,(char*)rptfile,(char*)outfile);
}


/*
 * refer to swmm_start
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_start
	(JNIEnv *env, jobject jobj, jboolean savedrpt){
	swmm_start(savedrpt);
}

/*
 * refer to swmm_start
 */
JNIEXPORT jdouble JNICALL Java_com_geoprocessing_model_swmm_JSWMM_step
	(JNIEnv *env, jobject jobj){
	double elapsedTime = 0.0;
	swmm_step(&elapsedTime);
	return elapsedTime;
}

/*
 * refer to swmm_report
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_report
	(JNIEnv * env, jobject jobj){
		return swmm_report();
}

/*
 * refer to swmm_end
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_end
	(JNIEnv *env, jobject jobj){
	return	swmm_end();
}

/*
 * refer to swmm_close
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_close
  (JNIEnv *env, jobject jobj){
  return swmm_close();
}

/*
 * refer to swmm_getError
 */
JNIEXPORT jstring JNICALL Java_com_geoprocessing_model_swmm_JSWMM_getError
	(JNIEnv *env, jobject jobj, jint errorCode){
	char *errMsg="";
	swmm_getError(errMsg,127);
	return charToJstring(env,errMsg);
}

/*
 * return current simualation time, a string format "year month day hour minute second"
 */
JNIEXPORT jstring JNICALL Java_com_geoprocessing_model_swmm_JSWMM_getCurrentSimualationTime
	(JNIEnv *env, jobject jobj){
		int year=0;
		int month=0;
		int day=0;
		int hour=0;
		int minute=0;
		int second=0;

		char * cyear="";
		char *cmonth="";
		char *cday="";
		char *chour="";
		char *cminute = "";
		char *csecond = "";
		char *ctime=(char *) malloc(20);
		

		swmm_getCurrentDateTime(&year, &month, &day,&hour, &minute, &second);

		//itoa(year,cyear, 10);
		sprintf(cyear, "%d", year);
		strcpy(ctime,cyear);
		strcat(ctime," ");

		
		//itoa(month,cmonth, 10);
		sprintf(cmonth, "%d", month);
		strcat(ctime,cmonth);
		strcat(ctime," ");
		
		//itoa(day,cday, 10);
		sprintf(cday, "%d", day);
		strcat(ctime,cday);
		strcat(ctime," ");

		
		//itoa(hour,chour, 10);
		sprintf(chour, "%d", hour);
		strcat(ctime,chour);
		strcat(ctime," ");

		
		//itoa(minute,cminute, 10);
		sprintf(cminute, "%d", minute);
		strcat(ctime,cminute);
		strcat(ctime," ");

		
		//itoa(second,csecond, 10);
		sprintf(csecond, "%d", second);
		strcat(ctime,csecond);

		return charToJstring(env,ctime);
}


/*
* @return the index (if >= 0) or the error code (if <0, needs to multiply by -1)
 */
JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_findObjectIndex
	(JNIEnv *env, jobject jobj, jint jtype, jstring jid){

		int index =0;
		const char* strId = (*env)->GetStringUTFChars(env, jid, NULL);
		int errorCode = swmm_project_findObject(jtype,(char *)strId,&index);

		if(errorCode!=0)
			return errorCode*(-1);

		return index;
}

/*
 * returns precipitation (if >=0) or error code (if <0, needs to multiply by -1)
 */
JNIEXPORT jdouble JNICALL Java_com_geoprocessing_model_swmm_JSWMM_getGagePrecip
	(JNIEnv *env, jobject jobj, jint jindex){

		double value[3]={0,0,0};
		double *gagearray = value;
		int errorCode = swmm_getGagePrecip(jindex,&gagearray);
		if(errorCode!=0)
			return errorCode*(-1);

		return gagearray[1];
}

JNIEXPORT jint JNICALL Java_com_geoprocessing_model_swmm_JSWMM_setGagePrecip
	(JNIEnv * env, jobject jobj, jint jindex, jdouble jprecipiation){
		return swmm_setGagePrecip(jindex,jprecipiation);
}

