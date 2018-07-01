using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace ViewerApp
{
    class Logging
    {

        static string strLogFilePath = string.Empty;
        private static StreamWriter sw = null;
        /// <summary>
        /// Setting LogFile path. If the logfile path is 
        /// null then it will update error info into LogFile.txt under
        /// application directory.
        /// </summary>
        public static string LogFilePath
        {
            set
            {
                strLogFilePath = value;
            }
            get
            {
                return strLogFilePath;
            }
        }
        /// <summary>
        /// Write Source,method,date,time,computer,error 
        /// and stack trace information to the text file
        /// 
        /// <param name="strPathName"></param>
        /// <param name="objException"></param>
        /// <RETURNS>false if the problem persists</RETURNS>
        ///</summary>
        public static bool WriteErrorLog(Exception objException)
        {
            string strPathName = GetLogFilePath();

            bool bReturn = false;
            string strException = string.Empty;
            try
            {
                sw = new StreamWriter(strPathName, true);
                sw.WriteLine("Source        : " +
                        objException.Source.ToString().Trim());
                sw.WriteLine("Method        : " +
                        objException.TargetSite.Name.ToString());
                sw.WriteLine("Date        : " +
                        DateTime.Now.ToLongTimeString());
                sw.WriteLine("Time        : " +
                        DateTime.Now.ToShortDateString());
                sw.WriteLine("Computer    : " +
                        Dns.GetHostName().ToString());
                sw.WriteLine("Error        : " +
                        objException.Message.ToString().Trim());
                sw.WriteLine("Stack Trace    : " +
                        objException.StackTrace.ToString().Trim());
                sw.WriteLine("^^------------------------ ------------------------------------------ -^^ ");
        
                sw.Flush();
                sw.Close();
                bReturn = true;
            }
            catch (Exception)
            {
                bReturn = false;
            }
            return bReturn;
        }


        /// <summary>
        /// Check the log file in applcation directory.
        /// If it is not available, creae it
        /// 
        /// <RETURNS>Log file path</RETURNS>
        ///</summary>
        private static string GetLogFilePath()
        {
            try
            {
                // get the base directory
                //string baseDir = AppDomain.CurrentDomain.BaseDirectory +
                //               AppDomain.CurrentDomain.RelativeSearchPath;

                //// search the file below the current directory
                //string retFilePath = baseDir + "//" + "LogFile.txt";
                //strLogFilePath = retFilePath;


                string retFilePath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "R3D", "Log.txt");

                if(!Directory.Exists(Path.GetDirectoryName(retFilePath)))
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(retFilePath));
                }


                // if exists, return the path
                if (File.Exists(retFilePath) == true)
                    return retFilePath;
                //create a text file
                else
                {
                    //if (false == CheckDirectory(retFilePath))
                    //    return string.Empty;

                    FileStream fs = new FileStream(retFilePath,
                          FileMode.OpenOrCreate, FileAccess.ReadWrite);
                    fs.Close();
                }

                return retFilePath;
            }
            catch (Exception)
            {
                return string.Empty;
            }
        }

        /// <summary>
        /// Create a directory if not exists
        /// 
        /// <param name="stLogPath"></param>
        ///<summary> <RETURNS></RETURNS>
        private static bool CheckDirectory(string strLogPath)
        {
            try
            {
                int nFindSlashPos = strLogPath.Trim().LastIndexOf("\\");
                string strDirectoryname =
                           strLogPath.Trim().Substring(0, nFindSlashPos);

                if (false == Directory.Exists(strDirectoryname))
                    Directory.CreateDirectory(strDirectoryname);
                return true;
            }
            catch (Exception)
            {
                return false;

            }
        }

    }
}
