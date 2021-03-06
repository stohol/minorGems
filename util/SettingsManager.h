/*
 * Modification History
 *
 * 2002-September-16    Jason Rohrer
 * Created.
 * Fixed a memory leak.
 *
 * 2002-September-25    Jason Rohrer
 * Added a setSetting function that takes a string value.
 *
 * 2002-September-26    Jason Rohrer
 * Added functions for integer values.
 *
 * 2003-August-24    Jason Rohrer
 * Fixed to remove 499-character limit for a setting value.
 *
 * 2009-February-11    Jason Rohrer
 * Made getSettingsFile public to support arbitry binary data in settings.
 *
 * 2009-February-12    Jason Rohrer
 * Added support for secure hashing.
 *
 * 2010-May-14    Jason Rohrer
 * String parameters as const to fix warnings.
 *
 * 2010-November-22    Jason Rohrer
 * More convenient getIntSetting call.
 *
 * 2011-January-3    Jason Rohrer
 * More convenient getFloatSetting call.
 *
 * 2017-April-13    Jason Rohrer
 * Functions for saving time settings in a future-proof format.
 *
 * 2017-November-1    Jason Rohrer
 * More convenient getStringSetting call (with default value).
 *
 * 2017-November-8    Jason Rohrer
 * getStringSetting now returns entire file contents.
 *
 * 2017-November-17    Jason Rohrer
 * getStringSetting back to returning first token, with new getSettingContents
 * function for getting entire file contents.
 *
 * 2019-March-15    Jason Rohrer
 * Support for returning list of ints from setting.
 */

#include "minorGems/common.h"



#ifndef SETTINGS_MANAGER_INCLUDED
#define SETTINGS_MANAGER_INCLUDED



#include "minorGems/util/SimpleVector.h"
#include "minorGems/system/MutexLock.h"

#include "minorGems/system/Time.h"

#include <stdio.h>



// utility class for dealing with static member dealocation
class SettingsManagerStaticMembers;



/**
 * Class that manages program settings.
 *
 * @author Jason Rohrer
 */
class SettingsManager {



    public:



        /**
         * Sets the directory name where settings are stored.
         *
         * @param inName the name of the directory (relative to the
         *   program's working directory).
         *   Must be destroyed by caller if non-const.
         */
        static void setDirectoryName( const char *inName );



        /**
         * Gets the directory name where settings are stored.
         *
         * @return the name of the directory (relative to the
         *   program's working directory).
         *   Must be destroyed by caller.
         */
        static char *getDirectoryName();



        /**
         * Sets the salt to be used when hashing settings.
         *
         * @param inSalt the salt as an ASCII string.
         *   Must be destroyed by caller if non-const.
         */
        static void setHashSalt( const char *inSalt );



        /**
         * Gets the salt to be used when hashing settings.
         *
         * @return the hash salt.
         *   Must be destroyed by caller.
         */
        static char *getHashSalt();


        
        /**
         * Turns hashing on or off.  If off, manager will reject
         * settings in folder that do not have proper hashes.
         *
         * @param inOn true to turn hashing on.
         */
        static void setHashingOn( char inOn );


        
        /**
         * Gets a setting, tokenized by whitespace into separate strings.
         *
         * @param inSettingName the name of the setting to get.
         *   Must be destroyed by caller if non-const.
         *
         * @return a vector of strings representing the setting value.
         *   The vector and the strings it contains must be destroyed
         *   by the caller.
         */
        static SimpleVector<char *> *getSetting( const char *inSettingName );

        static SimpleVector<int> *getIntSettingMulti(
            const char *inSettingName );

        static SimpleVector<float> *getFloatSettingMulti(
            const char *inSettingName );

        static SimpleVector<double> *getDoubleSettingMulti(
            const char *inSettingName );



        /**
         * Gets a string setting.  First whitespace-delimited value
         * found in the settings file
         *
         * @param inSettingName the name of the setting to get.
         *   Must be destroyed by caller if non-const.
         *         
         * @return the setting value string, or NULL if no setting
         *   value can be read.
         *   Must be destroyed by caller if non-NULL.
         */
        static char *getStringSetting( const char *inSettingName );



        // same as above, but never returns NULL
        // if setting cannot be read, returns copy of inDefaultValue
        static char *getStringSetting( const char *inSettingName,
                                       const char *inDefaultValue );


        /**
         * Gets full contents of a settings file as a string.
         *
         * Returns NULL on failure.
         *
         * Result destroyed by caller if non-NULL.
         */
        static char *getSettingContents( const char *inSettingName );
        
        
        // same as above, but never returns NULL
        // if setting cannot be read, returns copy of inDefaultValue
        static char *getSettingContents( const char *inSettingName,
                                         const char *inDefaultValue );



        /**
         * Gets a float setting.
         *
         * @param inSettingName the name of the setting to get.
         *   Must be destroyed by caller if non-const.
         * @param outValueFound pointer to where flag should be returned
         *   indicating whether or not the value was found.
         *   Set to true if the value was found, false otherwise.
         *         
         * @return the setting value.
         */
        static float getFloatSetting( const char *inSettingName,
                                      char *outValueFound );


        /**
         * Returns default value if setting not found.
         */
        static float getFloatSetting( const char *inSettingName,
                                      float inDefaultValue );



        /**
         * Gets a double setting.
         *
         * @param inSettingName the name of the setting to get.
         *   Must be destroyed by caller if non-const.
         * @param outValueFound pointer to where flag should be returned
         *   indicating whether or not the value was found.
         *   Set to true if the value was found, false otherwise.
         *         
         * @return the setting value.
         */
        static double getDoubleSetting( const char *inSettingName,
                                        char *outValueFound );


        /**
         * Returns default value if setting not found.
         */
        static double getDoubleSetting( const char *inSettingName,
                                        double inDefaultValue );


        
        /**
         * Gets an integer setting.
         *
         * @param inSettingName the name of the setting to get.
         *   Must be destroyed by caller if non-const.
         * @param outValueFound pointer to where flag should be returned
         *   indicating whether or not the value was found.
         *   Set to true if the value was found, false otherwise.
         *         
         * @return the setting value.
         */
        static int getIntSetting( const char *inSettingName,
                                  char *outValueFound );
        


        /**
         * Returns default value if setting not found.
         */
        static int getIntSetting( const char *inSettingName,
                                  int inDefaultValue );
        

        /**
         * Returns seconds since GMT epoch.
         */
        static timeSec_t getTimeSetting( const char *inSettingName,
                                         timeSec_t inDefaultValue );
        
        
        
        /**
         * Sets a setting.
         *
         * @param inSettingName the name of the setting to set.
         *   Must be destroyed by caller if non-const.
         * @param inSettingVector a vector of strings representing the
         *   setting value.
         *   The vector and the strings it contains must be destroyed
         *   by the caller.
         */
        static void setSetting( const char *inSettingName,
                                SimpleVector<char *> *inSettingVector );



        /**
         * Sets a setting to a single float value.
         *
         * @param inSettingName the name of the setting to set.
         *   Must be destroyed by caller if non-const.
         * @param inSettingValue the value to set.
         */
        static void setSetting( const char *inSettingName,
                                float inSettingValue );


        /**
         * Sets a setting to a single double value.
         *
         * Specialized name to differentiate from timesec_t version
         *
         * @param inSettingName the name of the setting to set.
         *   Must be destroyed by caller if non-const.
         * @param inSettingValue the value to set.
         */
        static void setDoubleSetting( const char *inSettingName,
                                      double inSettingValue );


        
        /**
         * Sets a setting to a single int value.
         *
         * @param inSettingName the name of the setting to set.
         *   Must be destroyed by caller if non-const.
         * @param inSettingValue the value to set.
         */
        static void setSetting( const char *inSettingName,
                                int inSettingValue );



        /**
         * Sets a setting to a time in seconds since the GMT epoch.
         *
         * This is written to file internally in a future-proof string
         * format (double).
         *
         * @param inSettingName the name of the setting to set.
         *   Must be destroyed by caller if non-const.
         * @param inSettingValue the value to set.
         */
        static void setSetting( const char *inSettingName,
                                timeSec_t inSettingValue );


        
        /**
         * Sets a setting to a single string value.
         *
         * @param inSettingName the name of the setting to set.
         *   Must be destroyed by caller if non-const.
         * @param inSettingValue the value to set.
         *   Must be destroyed by caller.
         */
        static void setSetting( const char *inSettingName,
                                const char *inSettingValue );

        

        /**
         * Gets the file for a setting name.
         *
         * @param inSettingName the name of the setting.
         *   Must be destroyed by caller if non-const.
         * @param inReadWriteFlags the flags to pass into the
         *   fopen call.  For example, "r" or "w".
         *
         * @return the file descriptor, or NULL if the open failed.
         *   Must be fclose()'d by caller if non-NULL.
         */
        static FILE *getSettingsFile( const char *inSettingName,
                                      const char *inReadWriteFlags );


        
    protected:


        
        static SettingsManagerStaticMembers mStaticMembers;

        static char mHashingOn;
        

        /**
         * Gets the file name for a setting with the default ini extension.
         * The .ini extension is added automatically by this call.
         *
         * @param inSettingName the name of the setting.
         *   Must be destroyed by caller if non-const.
         *
         * @return the name of the settings file.
         *   Must be destroyed by caller.
         */
        static char *getSettingsFileName( const char *inSettingName );
        

        /**
         * Gets the file name for a setting with a custom extension.
         * The extension should be passed in without a period in it.
         *
         * @param inSettingName the name of the setting.
         *   Must be destroyed by caller if non-const.
         * @param inExtension the extension to add to the name.
         *   Example:  "ini"
         *   Must be destroyed by caller if non-const.
         *
         * @return the name of the settings file.
         *   Must be destroyed by caller.
         */
        static char *getSettingsFileName( const char *inSettingName,
                                          const char *inExtension );
        
    };



/**
 * Container for static members to allow for their proper destruction
 * on program termination.
 *
 * @author Jason Rohrer
 */
class SettingsManagerStaticMembers {


        
    public:


        
        SettingsManagerStaticMembers();
        ~SettingsManagerStaticMembers();
        
        char *mDirectoryName;
        char *mHashSalt;
        


    };



#endif
