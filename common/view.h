#pragma once

#include "lc_glwidget.h"
#include "camera.h"
#include "lc_scene.h"
#include "lc_viewsphere.h"
#include "lc_commands.h"

enum class lcTrackButton
{
	None,
	Left,
	Middle,
	Right
};

enum class lcTrackTool
{
	None,
	Insert,
	PointLight,
	SpotLight,
	Camera,
	Select,
	MoveX,
	MoveY,
	MoveZ,
	MoveXY,
	MoveXZ,
	MoveYZ,
	MoveXYZ,
	RotateX,
	RotateY,
	RotateZ,
	RotateXY,
	RotateXYZ,
	ScalePlus,
	ScaleMinus,
	Eraser,
	Paint,
	ColorPicker,
	Zoom,
	Pan,
	OrbitX,
	OrbitY,
	OrbitXY,
	Roll,
	ZoomRegion,
	Count
};

class View : public lcGLWidget
{
public:
	View(lcModel* Model);
	~View();

	View(const View&) = delete;
	View(View&&) = delete;
	View& operator=(const View&) = delete;
	View& operator=(View&&) = delete;

	void Clear()
	{
		mModel = nullptr;
		mActiveSubmodelInstance = nullptr;
	}

	lcModel* GetModel() const
	{
		return mModel;
	}

	lcModel* GetActiveModel() const;
	void SetTopSubmodelActive();
	void SetSelectedSubmodelActive();

	static void CreateResources(lcContext* Context);
	static void DestroyResources(lcContext* Context);

	void OnDraw() override;
	void OnInitialUpdate() override;
	void OnUpdateCursor() override;
	void OnLeftButtonDown() override;
	void OnLeftButtonUp() override;
	void OnLeftButtonDoubleClick() override;
	void OnMiddleButtonDown() override;
	void OnMiddleButtonUp() override;
	void OnRightButtonDown() override;
	void OnRightButtonUp() override;
	void OnBackButtonUp() override;
	void OnForwardButtonUp() override;
	void OnMouseMove() override;
	void OnMouseWheel(float Direction) override;
	void BeginDrag(lcDragState DragState) override;
	void EndDrag(bool Accept) override;

	bool IsTracking() const
	{
		return mTrackButton != lcTrackButton::None;
	}

	void StartOrbitTracking();
	void CancelTrackingOrClearSelection();

	void SetProjection(bool Ortho);
	void LookAt();
	void ZoomExtents();
	void MoveCamera(const lcVector3& Direction);
	void Zoom(float Amount);

	void RemoveCamera();
	void SetCamera(lcCamera* Camera, bool ForceCopy);
	void SetCamera(const char* CameraName);
	void SetCameraIndex(int Index);
	void SetViewpoint(lcViewpoint Viewpoint);
	void SetViewpoint(const lcVector3& Position);
	void SetCameraAngles(float Latitude, float Longitude);
	void SetDefaultCamera();
	lcCursor GetCursor() const;
	void ShowContextMenu() const;

	lcVector3 GetMoveDirection(const lcVector3& Direction) const;
	lcMatrix44 GetPieceInsertPosition(bool IgnoreSelected, PieceInfo* Info) const;
	lcVector3 GetCameraLightInsertPosition() const;
	void GetRayUnderPointer(lcVector3& Start, lcVector3& End) const;
	lcObjectSection FindObjectUnderPointer(bool PiecesOnly, bool IgnoreSelected) const;
	lcArray<lcObject*> FindObjectsInBox(float x1, float y1, float x2, float y2) const;

	bool BeginRenderToImage(int Width, int Height);
	void EndRenderToImage();

	QImage GetRenderImage() const
	{
		return mRenderImage;
	}

protected:
	static void CreateSelectMoveOverlayMesh(lcContext* Context);

	void DrawSelectMoveOverlay();
	void DrawRotateOverlay();
	void DrawSelectZoomRegionOverlay();
	void DrawRotateViewOverlay();
	void DrawGrid();
	void DrawViewport();

	void UpdateTrackTool();
	bool IsTrackToolAllowed(lcTrackTool TrackTool, quint32 AllowedTransforms) const;
	lcTool GetCurrentTool() const;
	lcTrackTool GetOverrideTrackTool(Qt::MouseButton Button) const;
	float GetOverlayScale() const;
	void StartTracking(lcTrackButton TrackButton);
	void StopTracking(bool Accept);
	void OnButtonDown(lcTrackButton TrackButton);
	lcMatrix44 GetTileProjectionMatrix(int CurrentRow, int CurrentColumn, int CurrentTileWidth, int CurrentTileHeight) const;

	lcModel* mModel;
	lcPiece* mActiveSubmodelInstance;
	lcMatrix44 mActiveSubmodelTransform;

	lcScene mScene;
	lcDragState mDragState;
	lcTrackButton mTrackButton;
	lcTrackTool mTrackTool;
	bool mTrackToolFromOverlay;
	bool mTrackUpdated;
	int mMouseDownX;
	int mMouseDownY;
	lcVector3 mMouseDownPosition;
	PieceInfo* mMouseDownPiece;
	QImage mRenderImage;
	std::pair<lcFramebuffer, lcFramebuffer> mRenderFramebuffer;
	lcViewSphere mViewSphere;

	lcVertexBuffer mGridBuffer;
	int mGridSettings[7];

	static lcVertexBuffer mRotateMoveVertexBuffer;
	static lcIndexBuffer mRotateMoveIndexBuffer;
};

