[@bs.module]
external reactPaginate : ReasonReact.reactClass = "react-paginate";

type data = {. "selected": int};

let make =
    (
      ~pageCount: int,
      ~pageRangeDisplayed: int,
      ~marginPagesDisplayed: int,
      ~onPageChange: data => unit=(_) => (),
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=reactPaginate,
    ~props={
      "pageCount": pageCount,
      "pageRangeDisplayed": pageRangeDisplayed,
      "marginPagesDisplayed": marginPagesDisplayed,
      "onPageChange": onPageChange,
    },
    children,
  );
