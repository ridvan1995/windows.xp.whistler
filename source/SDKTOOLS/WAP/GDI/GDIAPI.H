
/*++

   gdiapi.h: Defines the list of apis for GDI32.dll

         NOTE:  This file is generated by WRAPPER code generator.

--*/
#ifdef ZGDI32
char *aszApiNames [] = {
    "AbortDoc",
    "AbortPath",
    "AddFontResourceA",
    "AddFontResourceW",
    "AngleArc",
    "AnimatePalette",
    "Arc",
    "ArcTo",
    "BeginPath",
    "BitBlt",
    "CancelDC",
    "Chord",
    "CloseEnhMetaFile",
    "CloseFigure",
    "CloseMetaFile",
    "CombineRgn",
    "CombineTransform",
    "CopyEnhMetaFileA",
    "CopyEnhMetaFileW",
    "CopyMetaFileA",
    "CopyMetaFileW",
    "CreateBitmap",
    "CreateBitmapIndirect",
    "CreateBrushIndirect",
    "CreateCompatibleBitmap",
    "CreateCompatibleDC",
    "CreateDCA",
    "CreateDCW",
    "CreateDIBPatternBrush",
    "CreateDIBPatternBrushPt",
    "CreateDIBitmap",
    "CreateDiscardableBitmap",
    "CreateEllipticRgn",
    "CreateEllipticRgnIndirect",
    "CreateEnhMetaFileA",
    "CreateEnhMetaFileW",
    "CreateFontA",
    "CreateFontIndirectA",
    "CreateFontIndirectW",
    "CreateFontW",
    "CreateHalftonePalette",
    "CreateHatchBrush",
    "CreateICA",
    "CreateICW",
    "CreateMetaFileA",
    "CreateMetaFileW",
    "CreatePalette",
    "CreatePatternBrush",
    "CreatePen",
    "CreatePenIndirect",
    "CreatePolyPolygonRgn",
    "CreatePolygonRgn",
    "CreateRectRgn",
    "CreateRectRgnIndirect",
    "CreateRoundRectRgn",
    "CreateScalableFontResourceA",
    "CreateScalableFontResourceW",
    "CreateSolidBrush",
    "DPtoLP",
    "DeleteDC",
    "DeleteEnhMetaFile",
    "DeleteMetaFile",
    "DeleteObject",
    "DrawEscape",
    "Ellipse",
    "EndDoc",
    "EndPage",
    "EndPath",
    "EnumEnhMetaFile",
    "EnumFontFamiliesA",
    "EnumFontFamiliesW",
    "EnumFontsA",
    "EnumFontsW",
    "EnumMetaFile",
    "EnumObjects",
    "EqualRgn",
    "Escape",
    "ExcludeClipRect",
    "ExtCreatePen",
    "ExtCreateRegion",
    "ExtEscape",
    "ExtFloodFill",
    "ExtSelectClipRgn",
    "ExtTextOutA",
    "ExtTextOutW",
    "FillPath",
    "FillRgn",
    "FixBrushOrgEx",
    "FlattenPath",
    "FloodFill",
    "FrameRgn",
    "GdiComment",
    "GdiFlush",
    "GdiGetBatchLimit",
    "GdiSetBatchLimit",
    "GetArcDirection",
    "GetAspectRatioFilterEx",
    "GetBitmapBits",
    "GetBitmapDimensionEx",
    "GetBkColor",
    "GetBkMode",
    "GetBoundsRect",
    "GetBrushOrgEx",
    "GetCharABCWidthsA",
    "GetCharABCWidthsFloatA",
    "GetCharABCWidthsFloatW",
    "GetCharABCWidthsW",
    "GetCharWidth32A",
    "GetCharWidth32W",
    "GetCharWidthA",
    "GetCharWidthFloatA",
    "GetCharWidthFloatW",
    "GetCharWidthW",
    "GetClipBox",
    "GetClipRgn",
    "GetColorAdjustment",
    "GetCurrentObject",
    "GetCurrentPositionEx",
    "GetDCOrgEx",
    "GetDIBits",
    "GetDeviceCaps",
    "GetEnhMetaFileA",
    "GetEnhMetaFileBits",
    "GetEnhMetaFileDescriptionA",
    "GetEnhMetaFileDescriptionW",
    "GetEnhMetaFileHeader",
    "GetEnhMetaFilePaletteEntries",
    "GetEnhMetaFileW",
    "GetFontData",
    "GetGlyphOutlineA",
    "GetGlyphOutlineW",
    "GetGraphicsMode",
    "GetKerningPairsA",
    "GetKerningPairsW",
    "GetMapMode",
    "GetMetaFileA",
    "GetMetaFileBitsEx",
    "GetMetaFileW",
    "GetMetaRgn",
    "GetMiterLimit",
    "GetNearestColor",
    "GetNearestPaletteIndex",
    "GetObjectA",
    "GetObjectType",
    "GetObjectW",
    "GetOutlineTextMetricsA",
    "GetOutlineTextMetricsW",
    "GetPaletteEntries",
    "GetPath",
    "GetPixel",
    "GetPolyFillMode",
    "GetROP2",
    "GetRasterizerCaps",
    "GetRegionData",
    "GetRgnBox",
    "GetStockObject",
    "GetStretchBltMode",
    "GetSystemPaletteEntries",
    "GetSystemPaletteUse",
    "GetTextAlign",
    "GetTextCharacterExtra",
    "GetTextColor",
    "GetTextExtentExPointA",
    "GetTextExtentExPointW",
    "GetTextExtentPoint32A",
    "GetTextExtentPoint32W",
    "GetTextExtentPointA",
    "GetTextExtentPointW",
    "GetTextFaceA",
    "GetTextFaceW",
    "GetTextMetricsA",
    "GetTextMetricsW",
    "GetViewportExtEx",
    "GetViewportOrgEx",
    "GetWinMetaFileBits",
    "GetWindowExtEx",
    "GetWindowOrgEx",
    "GetWorldTransform",
    "IntersectClipRect",
    "InvertRgn",
    "LPtoDP",
    "LineDDA",
    "LineTo",
    "MaskBlt",
    "ModifyWorldTransform",
    "MoveToEx",
    "OffsetClipRgn",
    "OffsetRgn",
    "OffsetViewportOrgEx",
    "OffsetWindowOrgEx",
    "PaintRgn",
    "PatBlt",
    "PathToRegion",
    "Pie",
    "PlayEnhMetaFile",
    "PlayEnhMetaFileRecord",
    "PlayMetaFile",
    "PlayMetaFileRecord",
    "PlgBlt",
    "PolyBezier",
    "PolyBezierTo",
    "PolyDraw",
    "PolyPolygon",
    "PolyPolyline",
    "PolyTextOutA",
    "PolyTextOutW",
    "Polygon",
    "Polyline",
    "PolylineTo",
    "PtInRegion",
    "PtVisible",
    "RealizePalette",
    "RectInRegion",
    "RectVisible",
    "Rectangle",
    "RemoveFontResourceA",
    "RemoveFontResourceW",
    "ResetDCA",
    "ResetDCW",
    "ResizePalette",
    "RestoreDC",
    "RoundRect",
    "SaveDC",
    "ScaleViewportExtEx",
    "ScaleWindowExtEx",
    "SelectClipPath",
    "SelectClipRgn",
    "SelectObject",
    "SelectPalette",
    "SetAbortProc",
    "SetArcDirection",
    "SetBitmapBits",
    "SetBitmapDimensionEx",
    "SetBkColor",
    "SetBkMode",
    "SetBoundsRect",
    "SetBrushOrgEx",
    "SetColorAdjustment",
    "SetDIBits",
    "SetDIBitsToDevice",
    "SetEnhMetaFileBits",
    "SetGraphicsMode",
    "SetMapMode",
    "SetMapperFlags",
    "SetMetaFileBitsEx",
    "SetMetaRgn",
    "SetMiterLimit",
    "SetPaletteEntries",
    "SetPixel",
    "SetPixelV",
    "SetPolyFillMode",
    "SetROP2",
    "SetRectRgn",
    "SetStretchBltMode",
    "SetSystemPaletteUse",
    "SetTextAlign",
    "SetTextCharacterExtra",
    "SetTextColor",
    "SetTextJustification",
    "SetViewportExtEx",
    "SetViewportOrgEx",
    "SetWinMetaFileBits",
    "SetWindowExtEx",
    "SetWindowOrgEx",
    "SetWorldTransform",
    "StartDocA",
    "StartDocW",
    "StartPage",
    "StretchBlt",
    "StretchDIBits",
    "StrokeAndFillPath",
    "StrokePath",
    "TextOutA",
    "TextOutW",
    "UnrealizeObject",
    "UpdateColors",
    "WidenPath",
    "ChoosePixelFormat",
    "CreateDIBSection",
    "GetDIBColorTable",
    "SetDIBColorTable",
    "GetPixelFormat",
    "SetPixelFormat",
        ""
};
#endif
