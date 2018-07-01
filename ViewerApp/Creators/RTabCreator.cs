using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Commands;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace ViewerApp.Creators
{
    public class RTabCreator
    {
        IUIManager _UIManager;

        public RTabCreator(IUIManager UIManager)
        {
            _UIManager = UIManager;
        }

        //public void CreateCommonUIButtons()
        //{
        //    var homeTab = _UIManager.CreateNewTab("Home");

        //    var buttonGroup =  homeTab.CreateButtonGroup("Manipulators");

        //    var button = buttonGroup.CreateToggleButton("Translate", _UIManager.Data.GetIconPath("translate.png"), new TranslateCommand(_UIManager.Data));


        //}


    }
}
