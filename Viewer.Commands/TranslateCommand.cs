using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Viewer.AdvancedTypes.Document;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;


namespace Viewer.Commands
{
    public class TranslateCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            //if (_data.ActiveDocument == null)
            //    return false;
            //else
                return true;
        }

        public void Execute(object parameter)
        {
            var ribbonToggleButton = parameter as IRibbonToggleButton;

            var doc = _data.ActiveDocument as RDocument;

            if (doc == null || parameter == null)
                return;
            
            if (ribbonToggleButton.IsChecked == true)
                doc.Editor.SetDraggerState(DraggerType.TranslateAxisDragger, true);
            else
                doc.Editor.SetDraggerState(DraggerType.TranslateAxisDragger, false);
        }

        IApplicationData _data;

        public TranslateCommand(IApplicationData data)
        {
            _data = data;
        }

    }
}
