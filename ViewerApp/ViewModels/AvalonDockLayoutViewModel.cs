using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
//using Edi.Events;
//using Edi.Interfaces;
//using ICSharpCode.AvalonEdit.Utils;
//using Microsoft.Practices.Composite.Events;
//using Microsoft.Practices.Prism.Commands;
using Xceed.Wpf.AvalonDock;
using Prism.Commands;
using ViewerApp.UIInterfaces;
using Prism.Events;
using ViewerApp.CustomEvents;
using ICSharpCode.AvalonEdit.Utils;

namespace ViewerApp.ViewModels
{
    class SaveLayoutEventArgs
    {
        public string XmlLayout { get; set; }
    }

    /// <summary>
    /// Class implements a viewmodel to support the
    /// <seealso cref="AvalonDockLayoutSerializer"/>
    /// attached behavior which is used to implement
    /// load/save of layout information on application
    /// start and shut-down.
    /// </summary>
    public class AvalonDockLayoutViewModel
    {
        #region fields
        private const string LayoutFileName = "Layout.config";

        readonly IEventAggregator _aggregator;

        private DelegateCommand mLoadWorkspaceLayoutFromStringCommand = null;
        private DelegateCommand mSaveWorkspaceLayoutToStringCommand = null;

        private DelegateCommand<object> mLoadLayoutCommand = null;
        private DelegateCommand<object> mSaveLayoutCommand = null;

        // The XML workspace layout string is stored in this field
        private string current_layout;

        ILayoutViewModelParent mParent = null;
        #endregion fields

        #region constructor
        /// <summary>
        /// Parameterized class constructor to model properties that
        /// are required for access to parent viewmodel.
        /// </summary>
        /// <param name="parent"></param>
        public AvalonDockLayoutViewModel(ILayoutViewModelParent parent, IEventAggregator events) : this()
        {
            this.mParent = parent;
            _aggregator = events;
        }

        /// <summary>
        /// Hidden class constructor
        /// </summary>
        protected AvalonDockLayoutViewModel()
        {
            this.current_layout = null;
        }
        #endregion

        #region command properties
        #region Load/Save workspace to temporary string
        /// <summary>
        /// Get an ICommand to save the WorkspaceLayout
        /// </summary>
        public ICommand SaveWorkspaceLayoutToStringCommand
        {
            get
            {
                // Save layout command can be executed at any time since there always is a layout that can be saved...
                if (this.mSaveWorkspaceLayoutToStringCommand == null)
                {
                    this.mSaveWorkspaceLayoutToStringCommand =
                       new DelegateCommand(this.SaveWorkspaceLayout_Executed,
                                           () => this.mParent.IsBusy == false);

                    CommandManager.RequerySuggested += (s, e) => this.mSaveWorkspaceLayoutToStringCommand.RaiseCanExecuteChanged();
                }

                return this.mSaveWorkspaceLayoutToStringCommand;
            }
        }

        /// <summary>
        /// Get an ICommand to load the WorkspaceLayout
        /// </summary>
        public ICommand LoadWorkspaceLayoutFromStringCommand
        {
            get
            {
                // Load layout command is not enabled unless the layout string is set.
                if (this.mLoadWorkspaceLayoutFromStringCommand == null)
                {
                    this.mLoadWorkspaceLayoutFromStringCommand = new DelegateCommand(this.LoadWorkspaceLayout_Executed,
                                                                                     () =>
                                                                                     this.mParent.IsBusy == false &&
                                                                                     string.IsNullOrEmpty(this.current_layout) == false);

                    CommandManager.RequerySuggested += (s, e) => this.mLoadWorkspaceLayoutFromStringCommand.RaiseCanExecuteChanged();
                }

                return this.mLoadWorkspaceLayoutFromStringCommand;
            }
        }
        #endregion Load/Save workspace to temporary string

        #region Load Save WorkSpace on application start-up and shutdown
        /// <summary>
        /// Implement a command to load the layout of an AvalonDock-DockingManager instance.
        /// This layout defines the position and shape of each document and tool window
        /// displayed in the application.
        /// 
        /// Parameter:
        /// The command expects a reference to a <seealso cref="DockingManager"/> instance to
        /// work correctly. Not supplying that reference results in not loading a layout (silent return).
        /// </summary>
        public ICommand LoadLayoutCommand
        {
            get
            {
                if (this.mLoadLayoutCommand == null)
                {
                    this.mLoadLayoutCommand = new DelegateCommand<object>((p) =>
                    {
                        DockingManager docManager = p as DockingManager;

                        if (docManager == null)
                            return;

                        try
                        {
                            this.LoadDockingManagerLayout();
                        }
                        catch
                        {
                        }
                        finally
                        {
                            this.mParent.IsBusy = false;
                        }
                    });
                }

                return this.mLoadLayoutCommand;
            }
        }

        /// <summary>
        /// Implements a command to save the layout of an AvalonDock-DockingManager instance.
        /// This layout defines the position and shape of each document and tool window
        /// displayed in the application.
        /// 
        /// Parameter:
        /// The command expects a reference to a <seealso cref="string"/> instance to
        /// work correctly. The string is supposed to contain the XML layout persisted
        /// from the DockingManager instance. Not supplying that reference to the string
        /// results in not saving a layout (silent return).
        /// </summary>
        public ICommand SaveLayoutCommand
        {
            get
            {
                if (this.mSaveLayoutCommand == null)
                {
                    this.mSaveLayoutCommand = new DelegateCommand<object>((p) =>
                    {
                        string xmlLayout = p as string;

                        if (xmlLayout == null)
                            return;

                        this.SaveDockingManagerLayout(xmlLayout);
                    });
                }

                return this.mSaveLayoutCommand;
            }
        }
        #endregion Load Save WorkSpace on application start-up and shutdown
        #endregion command properties

        #region methods
        #region Workspace Managment Methods
        /// <summary>
        /// This method is executed if the corresponding
        /// Save Workspace Layout command is executed.
        /// </summary>
        private void SaveWorkspaceLayout_Executed()
        {
            string xmlLayout = string.Empty;

            SaveLayoutEventArgs s = new SaveLayoutEventArgs();
            SynchronousEvent<SaveLayoutEventArgs>.Instance.Publish(s);

            this.current_layout = s.XmlLayout;
        }

        /// <summary>
        /// This method is executed if the corresponding
        /// Load Workspace Layout command is executed.
        /// </summary>
        private void LoadWorkspaceLayout_Executed()
        {
            // Is there any layout that could possible be loaded?
            if (string.IsNullOrEmpty(current_layout) == true)
                return;

            // Sends a LoadWorkspaceLayout message to registered recipients. The message will reach all recipients
            // that registered for this message type using one of the Register methods.
            // Messenger.Default.Send(new NotificationMessage<string>(current_layout, Notifications.LoadWorkspaceLayout));

            LoadLayoutEvent.Instance.Publish(current_layout);
        }
        #endregion Workspace Managment Methods

        #region LoadLayout
        /// <summary>
        /// Loads the layout of a particular docking manager instance from persistence
        /// and checks whether a file should really be reloaded (some files may no longer
        /// be available).
        /// </summary>
        private void LoadDockingManagerLayout()
        {
            string layoutFileName = string.Empty;

            try
            {
                this.mParent.IsBusy = true;

                layoutFileName = System.IO.Path.Combine(this.mParent.DirAppData, AvalonDockLayoutViewModel.LayoutFileName);

                if (System.IO.File.Exists(layoutFileName) == false)
                {
                    this.mParent.IsBusy = false;
                    return;
                }

                string sTaskError = string.Empty;

                Task taskToProcess = null;
                taskToProcess = Task.Factory.StartNew<string>((stateObj) =>
                {
                    string xml = string.Empty;

                    try
                    {
                        // Begin Aysnc Task
                        using (FileStream fs = new FileStream(layoutFileName, FileMode.Open, FileAccess.Read, FileShare.Read))
                        {
                            using (StreamReader reader = FileReader.OpenStream(fs, Encoding.Default))
                            {
                                xml = reader.ReadToEnd();
                            }
                        }
                    }
                    catch (OperationCanceledException exp)
                    {
                        throw exp;
                    }
                    catch (Exception except)
                    {
                        throw except;
                    }
                    finally
                    {
                    }

                    return xml;                     // End of async task

                }, null).ContinueWith(ant =>
                {
                    try
                    {
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            LoadLayoutEvent.Instance.Publish(ant.Result);
                        }),
                        DispatcherPriority.Background);
                    }
                    catch (AggregateException aggExp)
                    {
                        throw new Exception("One or more errors have occured during load layout processing.", aggExp);
                    }
                    finally
                    {
                        this.mParent.IsBusy = false;
                    }
                });
            }
            catch (Exception exp)
            {
                throw exp;
            }
        }
        #endregion LoadLayout

        #region SaveLayout
        private void SaveDockingManagerLayout(string xmlLayout)
        {
            // Create XML Layout file on close application (for re-load on application re-start)
            if (xmlLayout == null)
                return;

            string fileName = System.IO.Path.Combine(this.mParent.DirAppData, AvalonDockLayoutViewModel.LayoutFileName);

            File.WriteAllText(fileName, xmlLayout);
        }
        #endregion SaveLayout
        #endregion methods
    }
}