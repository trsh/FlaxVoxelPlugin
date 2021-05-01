#if FLAX_EDITOR
using FlaxEditor;
using FlaxEngine;
using System;


/*enum SDFModel {
    Sphere = 1,
    Box = 2
}*/

namespace Voxels
{
    /// <summary>
    /// Editor plugin for creating voxel geometry.
    /// </summary>
    /// <seealso cref="FlaxEditor.EditorPlugin" />
    public class VoxelEditor : EditorPlugin
    {
        /*private Tabs toolboxSpawnTabs;
        private Tab voxelPrimitivesTab;

        private class Item : TreeNode
        {
            private DragData _dragData;

            public Item(string text, DragData dragData = null)
            : this(text, dragData, FlaxEngine.SpriteHandle.Invalid)
            {
            }

            private Item(string text, DragData dragData, FlaxEngine.SpriteHandle icon)
            : base(false, icon, icon)
            {
                Text = text;
                Height = 20;
                TextMargin = new Margin(-5.0f, 2.0f, 2.0f, 2.0f);
                _dragData = dragData;
            }

            /// <inheritdoc />
            public override void Draw()
            {
                base.Draw();
            }

            /// <inheritdoc />
            protected override void DoDragDrop()
            {
                if (_dragData != null)
                {
                    DoDragDrop(_dragData);
                }
            }

            /// <inheritdoc />
            public override void OnDestroy()
            {
                _dragData = null;
                base.OnDestroy();
            }
        }

        /// <inheritdoc />
        public override void InitializeEditor()
        {
            base.InitializeEditor();

            toolboxSpawnTabs = Editor.Windows.ToolboxWin.Spawn.GetChild<Tabs>();

            voxelPrimitivesTab = new Tab("Voxel primitives");
            voxelPrimitivesTab = toolboxSpawnTabs.AddTab(voxelPrimitivesTab);

            var voxelPrimitivesPanel = new Panel(ScrollBars.Both)
            {
                AnchorPreset = AnchorPresets.StretchAll,
                Offsets = new Margin(0, 0, 0, 0),
                Parent = voxelPrimitivesTab
            };
            var voxelPrimitivesList = new Tree(false)
            {
                AnchorPreset = AnchorPresets.HorizontalStretchTop,
                IsScrollable = true,
                Parent = voxelPrimitivesPanel
            };

            var spherePirmitiveItem = new Item("Sphere", FlaxEditor.GUI.Drag.DragActorType.GetDragData(typeof(VoxelSphere)));

            voxelPrimitivesList.AddChild(spherePirmitiveItem);

            // EXP

            Level.SceneLoaded += OnSceneLoaded;
            //Scripting.Update += OnUpdate;
        }*/

        public override void InitializeEditor()
        {
            base.InitializeEditor();
            Level.SceneLoaded += OnSceneLoaded;
        }

        //Volume v = null;

        private void OnSceneLoaded(Scene arg1, Guid arg2)
        {
            /*if (Level.Scenes.Length > 0)
            {
                Camera cam = Level.Scenes[0].GetChild<Camera>();

                if (cam)
                {
                    Volume v = new Volume();
                    v.Init(new VolumeConfig
                    {
                        maxResolution = 16,
                        origin = new Int3(256, 256, 256),
                        size = 512,
                        observerCam = cam,
                        lodDistances = new int[] { 200, 400, 600 }
                    }, MesherType.MC);

                    v.FromSDF();
                }
            }*/
            Level.SceneLoaded -= OnSceneLoaded;
        }

        /*private void OnUpdate()
        {
            Camera cam = Level.Scenes[0].GetChild<Camera>();
            v.UpdateObserverCam(cam);
        }*/

        /// <inheritdoc />
        public override void Deinitialize()
        {

            /*if (voxelPrimitivesTab != null)
            {
                toolboxSpawnTabs.RemoveChild(voxelPrimitivesTab);
            }*/

            Level.SceneLoaded -= OnSceneLoaded;
            //Scripting.Update -= OnUpdate;
            base.Deinitialize();
        }
    }

    /*[CustomEditor(typeof(VoxelSphere))]
    public class VoxelPropsEditor : GenericEditor
    {
        public override void Initialize(LayoutElementsContainer layout)
        {
            layout.Label("Voxel object properties & tools", TextAlignment.Center);
            var propsGroup = layout.Group("Properties");

            base.Initialize(propsGroup);

            layout.Space(20);

            var toolsGroup = layout.Group("Tools");
            var sdfChooser = toolsGroup.Enum(typeof(SDFModel));
            var addSDFBtn = toolsGroup.Button("Add SDF form", Color.Green);

            addSDFBtn.Button.Clicked += () => addSDFModel(sdfChooser.ComboBox.Value);
        }

        private void addSDFModel(int sdfModelIdx)
        {
            if (sdfModelIdx == (int) SDFModel.Sphere.GetHashCode())
            {
                Debug.Log("add sphere");
                VoxelSphere inst = (VoxelSphere)Values[0];
                inst.AddSDFForm(SDFFormType.Sphere);
            }
            else if (sdfModelIdx == (int)SDFModel.Box.GetHashCode())
            {
                Debug.Log("add box");
                VoxelSphere inst = (VoxelSphere)Values[0];
                inst.AddSDFForm(SDFFormType.Box);
            }
        }
    }

    [CustomEditor(typeof(StaticModel))]
    public class VoxelModelPropsEditor : GenericEditor
    {
        public override void Initialize(LayoutElementsContainer layout)
        {

            StaticModel sm = (StaticModel)Values[0];

            if (sm.Parent.GetType().ToString() == "Voxels.VoxelSphere") { 
                //layout.Label("Basic", TextAlignment.Center);
                var propsGroup = layout.Group("Basic");

                base.Initialize(propsGroup);

                layout.Space(20);

                //layout.Label("Voxel", TextAlignment.Center);
                var toolsGroup = layout.Group("Voxel");
                //var addSDFBtn = toolsGroup.Button("Add SDF form", Color.Green);
                var sdfChooser = toolsGroup.Enum(typeof(SDFModel));
                sdfChooser.ComboBox.SelectedIndex = 0;
            }
            else
            {
                base.Initialize(layout);
            }
        }

    }

    [CustomEditor(typeof(SDFForm))]
    public class SDFFormPropsEditor : GenericEditor
    {
        public override void Initialize(LayoutElementsContainer layout)
        {

            base.Initialize(layout);
        }

    }*/
}
#endif