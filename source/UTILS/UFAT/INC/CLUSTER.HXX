/*++

Copyright (c) 1990 Microsoft Corporation

Module Name:

    cluster.hxx

Abstract:

    This class models a chain of clusters in a FAT file system.  It gives
    the ability to refer to a scattered chain of clusters as one contiguous
    region of memory.  This memory will be acquired from a MEM object at
    initialization.

Author:

    Norbert P. Kusters (norbertk) 27-Nov-90

--*/

#if !defined(CLUSTER_CHAIN_DEFN)

#define CLUSTER_CHAIN_DEFN


#include "secrun.hxx"
#include "hmem.hxx"

//
//    Forward references
//

DECLARE_CLASS( CLUSTER_CHAIN );
DECLARE_CLASS( FAT );
DECLARE_CLASS( FAT_SA );
DECLARE_CLASS( LOG_IO_DP_DRIVE );
DECLARE_CLASS( MEM );

class CLUSTER_CHAIN : public OBJECT {

    public:

        DECLARE_CONSTRUCTOR( CLUSTER_CHAIN );

        VIRTUAL
        ~CLUSTER_CHAIN(
            );

        NONVIRTUAL
        BOOLEAN
        Initialize(
            IN OUT  PMEM                Mem,
            IN OUT  PLOG_IO_DP_DRIVE    Drive,
            IN      PFAT_SA             FatSuperArea,
            IN      PCFAT               Fat,
            IN      USHORT              ClusterNumber,
            IN      USHORT              LengthOfChain DEFAULT 0
            );

        VIRTUAL
        BOOLEAN
        Read(
            );

        VIRTUAL
        BOOLEAN
        Write(
            );

        NONVIRTUAL
        PVOID
        GetBuf(
            );

        NONVIRTUAL
        USHORT
        QueryLength(
            ) CONST;

    private:

        NONVIRTUAL
        VOID
        Construct (
            );

        NONVIRTUAL
        VOID
        Destroy(
            );

        PSECRUN*    _secruns;
        USHORT      _num_secruns;
        USHORT      _length_of_chain;

        //
        // Stuff needed for compressed volumes.
        //

        BOOLEAN     _is_compressed;
        PSECRUN     _secrun;
        PUCHAR      _buf;
        HMEM        _hmem;
        PFAT_SA     _fat_sa;
        PCFAT       _fat;
        PLOG_IO_DP_DRIVE
                    _drive;
        USHORT      _starting_cluster;
};


INLINE
PVOID
CLUSTER_CHAIN::GetBuf(
    )
/*++

Routine Description:

    This routine returns a pointer to the beginning of the memory map for
    the cluster chain.

Arguments:

    None.

Return Value:

    A pointer to the beginning of the memory map for the cluster chain.

--*/
{
    if (_is_compressed) {
        return _buf;
    }
    return (_secruns && _secruns[0]) ? _secruns[0]->GetBuf() : NULL;
}


INLINE
USHORT
CLUSTER_CHAIN::QueryLength(
    ) CONST
/*++

Routine Description:

    Computes the number of clusters in the cluster chain.

Arguments:

    None.

Return Value:

    The number of clusters in the cluster chain.

--*/
{
    return _length_of_chain;
}


#endif // CLUSTER_CHAIN_DEFN
