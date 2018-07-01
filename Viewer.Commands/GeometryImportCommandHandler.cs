using Prism.Events;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using Viewer.AdvancedTypes.Document;
using Viewer.Interfaces;
using ViewerGraphicsEditor;

namespace Viewer.Commands
{
    public class GeometryImportCommandHandler : ICommand
    {
        //private IEditorInterop _editor;
        private IEventAggregator _eventAggregator;

        //private GeometryImportCommandHandler(IEditorInterop _editor)
        //{
        //    this._editor = _editor;
        //}
        IApplicationData _data;

        public GeometryImportCommandHandler(IApplicationData data, IEventAggregator _eventAggregator)
        {
            _data = data;
            this._eventAggregator = _eventAggregator;

        }

        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            _eventAggregator.GetEvent<Viewer.Infrastructure.StatusBarMessageEvent>().Publish("");


            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.Title = "Browse 3D Geometry Files";

            openFileDialog1.CheckFileExists = true;
            openFileDialog1.CheckPathExists = true;

            //openFileDialog1.DefaultExt = "obj";
            openFileDialog1.Filter = "Geometry files |" + _data.GetImportSupportFileTypeFilter();
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            //openFileDialog1.ReadOnlyChecked = true;
            //openFileDialog1.ShowReadOnly = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Stopwatch sw = new Stopwatch();
                sw.Start();
                string filename = openFileDialog1.FileName.Replace("\\", "/");


                var doc = _data.ActiveDocument as RDocument;

                var editor = doc.Editor as IEditorInterop;

                var childgeomNode = editor.ImportGeometry(openFileDialog1.FileName);

                if (childgeomNode == null)
                {

                    _eventAggregator.GetEvent<Viewer.Infrastructure.StatusBarMessageEvent>().Publish("Import Failed");
                    return;
                }

                sw.Stop();

                if (childgeomNode is IRTransformNode)
                {
                    var builderDoc = doc as IRBuilderDocument;
                    builderDoc.Tree.GeometryParentNode.AddChild(childgeomNode);
                    //editor.SetFocus(childgeomNode);
                }
                else if (childgeomNode is IRGeometryNode)
                {
                    var transNode = editor.CreateTransformNode(Path.GetFileNameWithoutExtension(openFileDialog1.FileName) + "_Transform");

                    transNode.AddChild(childgeomNode);

                    var builderDoc = doc as IRBuilderDocument;

                    builderDoc.Tree.GeometryParentNode.AddChild(transNode);
                    //editor.SetFocus(transNode);
                }

                var builderDosc = doc as IRBuilderDocument;

                var mm =  builderDosc.GetModelingManager();
                string cc =  mm.GetNumberOfDrawables();


                string elapsedTime = sw.Elapsed.ToString(@"hh\:mm\:ss");
                _eventAggregator.GetEvent<Viewer.Infrastructure.StatusBarMessageEvent>().Publish("Import Completed in " + elapsedTime);


                _eventAggregator.GetEvent<Viewer.Infrastructure.StatusBarMessageEvent>().Publish("Drawables Count " + cc);


            }
        }
    }
}
